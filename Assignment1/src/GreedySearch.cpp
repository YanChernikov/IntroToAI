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
	byte* goalState = puzzle.GetGoalState();
	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();

	Node* root = new Node(puzzle.GetState(), width, height, nullptr);
	root->hCost = CalculateHeuristic(root->state, goalState, width, height);
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);

	std::vector<Node*> closedSet, openSet;
	openSet.push_back(root);
	while (openSet.size() > 0)
	{
		m_Iterations++;
		int index = 0;
		Node* current = FindBestNode(openSet, &index);
		if (IsSolved(current->state, goalState, width * height))
			return TracePath(*current);

		openSet.erase(openSet.begin() + index);
		closedSet.push_back(current);
		std::vector<Node*> candidates = current->GetNextStates();
		for (Node* candidate : candidates)
		{
			if (SetContains(closedSet, candidate))
				continue;
			if (!SetContains(openSet, candidate))
				openSet.push_back(candidate);

			candidate->hCost = CalculateHeuristic(candidate->state, goalState, width, height);
		}
	}
	return std::vector<Direction>();
}

Node* GreedySearch::FindBestNode(const std::vector<Node*>& set, int* index)
{
	Node* result = nullptr;
	int min = 100000;
	for (int i = 0; i < set.size(); i++)
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

bool GreedySearch::SetContains(const std::vector<Node*>& set, Node* node)
{
	for (Node* n : set)
	{
		if (*n == *node)
			return true;
	}
	return false;
}

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