#include "IDAStarSearch.h"
#include "Puzzle.h"

#include <stack>

IDAStarSearch::IDAStarSearch()
{
}

std::vector<Direction> IDAStarSearch::TracePath(const Node& leaf)
{
	std::vector<Direction> results;
	const Node* node = &leaf;
	while (node->parent)
	{
		results.push_back(node->direction);
		node = node->parent;
	}
	return results;

}

std::vector<Direction> IDAStarSearch::Solve(const Puzzle& puzzle)
{
	m_Iterations = 0;

	int blankTileIndex = puzzle.FindBlankTile();
	byte* goalState = puzzle.GetGoalState();
	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();

	Node root((State*)puzzle.GetState(), width, height, nullptr);
	root.SetCost(0, CalculateHeuristic(root.state.values, goalState, width, height));
	root.position = Vector2i(blankTileIndex % width, blankTileIndex / width);

	std::vector<Node*> nodeList;

	float bound = root.hCost;
	while (true)
	{
		m_VisitedNodes.clear();
		std::stack<Node*> nodeStack;
		std::stack<float> boundStack;
		nodeStack.push(&root);
		boundStack.push(bound);

		float b = 0.0f;
		while (nodeStack.size() > 0)
		{
			m_Iterations++;
			Node* current = nodeStack.top();
			nodeStack.pop();
			b = boundStack.top();
			boundStack.pop();
			float f = current->gCost + CalculateHeuristic(current->state.values, goalState, width, height);
			if (f > b)
			{
				b = f;
				continue;
			}

			if (IsSolved(current->state.values, goalState, width * height))
			{
				std::vector<Direction> result = TracePath(*current);
				for (Node* n : nodeList)
					delete n;
				return result;
			}

			std::vector<Node*> candidates;
			std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
			std::function<void(Node*)> addCandidate = [&](Node* n)
			{
				m_VisitedNodes.insert(n->state);
				candidates.push_back(n);
			};

			current->GetNextStates(candidateCheck, addCandidate);
			for (Node* candidate : candidates)
			{
				int gCost = current->gCost + 1;
				candidate->SetCost(gCost, b);
				nodeStack.push(candidate);
				nodeList.push_back(candidate);
				boundStack.push(b);
			}
		}
		bound = b;
	}
	for (Node* n : nodeList)
		delete n;
	return std::vector<Direction>();
}

short IDAStarSearch::CalculateHeuristic(byte* state, byte* goalState, byte width, byte height)
{
	int score = 0;
	for (int y = 0; y < width * height; y++)
	{
		for (int x = 0; x < width * height; x++)
		{
			if (state[y] == goalState[x])
			{
				score += abs((x % width) - (y % width));
				score += abs((x / width) - (y / width));
			}

		}
	}
	return score;
}