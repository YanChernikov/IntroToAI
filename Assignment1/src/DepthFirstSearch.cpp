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
	Node* root = new Node((State*)puzzle.GetState(), width, height, nullptr);
	int blankTileIndex = puzzle.FindBlankTile();
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);
	std::stack<Node*> searchNodes;
	searchNodes.push(root);
	byte* goalState = puzzle.GetGoalState();

	std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
	std::function<void(Node*)> addCandidate = [&](Node* n)
	{
		m_VisitedNodes.insert(n->state);
		searchNodes.push(n);
	};

	while (searchNodes.size() > 0)
	{
		m_Iterations++;
		Node& node = *searchNodes.top();
		searchNodes.pop();

		if (IsSolved(node.state.values, goalState, width * height))
			return TracePath(node);

		node.GetNextStates(candidateCheck, addCandidate);

		if (node.RefCount == 0)
			delete &node;
	}
	return std::vector<Direction>();
}

bool DepthFirstSearch::IsNodeVisited(Node* node)
{
	return m_VisitedNodes.find(node->state) != m_VisitedNodes.end();
}