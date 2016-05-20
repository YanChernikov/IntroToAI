#include "GreedySearch.h"
#include "Puzzle.h"

GreedySearch::GreedySearch()
{
}

std::vector<Direction> GreedySearch::TracePath(const Node& leaf)
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

std::vector<Direction> GreedySearch::Solve(const Puzzle& puzzle)
{
	int blankTileIndex = puzzle.FindBlankTile();
	byte* goalState = puzzle.GetGoalState()->GetValues();
	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();

	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);
	root->hCost = CalculateHeuristic(root->GetState().GetValues(), goalState, width, height);
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);

	// Store every node we allocate as a unique pointer so that we can delete all of them when we exit this function
	std::vector<std::unique_ptr<Node>> nodeList;
	std::vector<Node*> candidates;
	std::vector<Node*> closedSet, openSet;
	std::function<bool(State&)> candidateCheck = [&](State& s)
	{
		return SetContains(closedSet, s);
	};

	std::function<void(Node*)> addCandidate = [&](Node* n)
	{
		m_VisitedNodes.insert(n->GetState());
		candidates.push_back(n);
		nodeList.push_back(std::unique_ptr<Node>(n));
	};

	openSet.push_back(root);
	while (openSet.size() > 0)
	{
		m_Iterations++;
		int index = 0;
		Node* current = FindBestNode(openSet, &index);
		if (IsSolved(current->GetState().GetValues(), goalState, width * height))
			return TracePath(*current);

		openSet.erase(openSet.begin() + index);
		closedSet.push_back(current);
		current->GetNextStates(candidateCheck, addCandidate);
		for (Node* candidate : candidates)
		{
			if (!SetContains(openSet, candidate->GetState()))
				openSet.push_back(candidate);

			candidate->hCost = CalculateHeuristic(candidate->GetState().GetValues(), goalState, width, height);
		}
		candidates.clear();
	}
	return std::vector<Direction>();
}

// This function finds the node with the lowest fCost in the given vector,
// and optionally provides its index in that vector
Node* GreedySearch::FindBestNode(const std::vector<Node*>& set, int* index)
{
	Node* result = nullptr;
	int min = 100000;
	for (uint i = 0; i < set.size(); i++)
	{
		Node* node = set[i];
		if (node->hCost < min)
		{
			min = node->hCost;
			result = node;
			if (index)
				*index = i;
		}
	}
	return result;
}

// Calculates the A* heuristic. I've chosen to use the sum of the Manhattan distances of each
// tile's position to its destination position in the puzzle, because this seemed to work really
// well in my testing
short GreedySearch::CalculateHeuristic(byte* state, byte* goalState, byte width, byte height)
{
	short score = 0;
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