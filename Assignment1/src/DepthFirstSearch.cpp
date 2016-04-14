#include "DepthFirstSearch.h"
#include "Puzzle.h"

#include <stack>

DepthFirstSearch::DepthFirstSearch()
{
}

std::vector<Direction> DepthFirstSearch::TracePath(const Node& leaf)
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

std::vector<Direction> DepthFirstSearch::Solve(const Puzzle& puzzle)
{
	m_Puzzle = &puzzle;

	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();
	Node* root = new Node(puzzle.GetState(), width, height, nullptr);
	int blankTileIndex = puzzle.FindBlankTile();
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);
	std::stack<Node*> searchNodes;
	searchNodes.push(root);
	byte* goalState = puzzle.GetGoalState();
	while (searchNodes.size() > 0)
	{
		m_Iterations++;
		Node& node = *searchNodes.top();
		searchNodes.pop();

		if (IsSolved(node.state, goalState, width * height))
			return TracePath(node);

		std::vector<Node*> candidates = node.GetNextStates();
		for (Node* candidate : candidates)
		{
			if (!IsNodeVisited(candidate))
			{
				m_VisitedNodes.push_back(candidate);
				searchNodes.push(candidate);
			}
			else
			{
				delete candidate;
			}
		}
	}
	while (Node* node = searchNodes.top())
	{
		delete node;
		searchNodes.pop();
	}
	return std::vector<Direction>();
}

bool DepthFirstSearch::IsNodeVisited(Node* node)
{
	for (Node* n : m_VisitedNodes)
	{
		if (*n == *node)
			return true;
	}
	return false;
}