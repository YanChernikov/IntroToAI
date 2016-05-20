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
	byte* goalState = puzzle.GetGoalState()->GetValues();
	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();

	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);
	root->SetCost(0, CalculateHeuristic(root->GetState().GetValues(), goalState, width, height));
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);

	struct Stack
	{
		Node* node;
		short bound;
	};

	// Store every node we allocate as a unique pointer so that we can delete all of them when we exit this function
	std::vector<std::unique_ptr<Node>> nodeList;
	std::vector<Node*> candidates;

	std::function<void(Node*)> addCandidate = [&](Node* n)
	{
		m_VisitedNodes.insert(n->GetState());
		candidates.push_back(n);
		nodeList.push_back(std::unique_ptr<Node>(n));
	};

	short searchBound = root->hCost;
	while (true)
	{
		m_VisitedNodes.clear();
		std::stack<Stack> stack;
		stack.push({ root, searchBound });

		short bound = 0;
		while (stack.size() > 0)
		{
			m_Iterations++;
			Stack s = stack.top();
			stack.pop();
			bound = s.bound;
			Node* current = s.node;
			short fCost = current->gCost + CalculateHeuristic(current->GetState().GetValues(), goalState, width, height);
			if (fCost > bound)
			{
				bound = fCost;
				continue;
			}

			if (IsSolved(current->GetState().GetValues(), goalState, width * height))
				return TracePath(*current);

			candidates.clear();
			std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
			current->GetNextStates(candidateCheck, addCandidate);
			for (Node* candidate : candidates)
			{
				int gCost = current->gCost + 1;
				candidate->SetCost(gCost, bound);
				stack.push({ candidate, bound });
			}
		}
		searchBound = bound;
	}
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