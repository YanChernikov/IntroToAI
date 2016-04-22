#include "IDDepthFirstSearch.h"
#include "Puzzle.h"

#include <stack>

IDDepthFirstSearch::IDDepthFirstSearch()
{
}

std::vector<Direction> IDDepthFirstSearch::TracePath(const Node& leaf)
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

std::vector<Direction> IDDepthFirstSearch::Solve(const Puzzle& puzzle)
{
	m_Puzzle = &puzzle;
	m_Iterations = 0;

	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();
	Node root((State*)puzzle.GetState(), width, height, nullptr);
	int blankTileIndex = puzzle.FindBlankTile();
	root.position = Vector2i(blankTileIndex % width, blankTileIndex / width);
	byte* goalState = puzzle.GetGoalState();
	for (int searchDepth = 0; ; searchDepth++)
	{
		std::vector<Node*> nodeList;
		m_VisitedNodes.clear();
		std::stack<Node*> searchNodes;
		std::stack<int> depthStack;
		searchNodes.push(&root);
		int depth = searchDepth;
		depthStack.push(depth);
		std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
		std::function<void(Node*)> addCandidate = [&](Node* n)
		{
			m_VisitedNodes.insert(n->state);
			searchNodes.push(n);
			nodeList.push_back(n);
		};

		while (searchNodes.size() > 0)
		{
			m_Iterations++;
			Node& node = *searchNodes.top();
			searchNodes.pop();

			int d = depthStack.top();
			depthStack.pop();

			if (d == 0 && IsSolved(node.state.values, goalState, width * height))
			{
				std::vector<Direction> result = TracePath(node);
				for (Node* n : nodeList)
					delete n;
				return result;
			}

			if (d <= 0)
				continue;

			int ps = searchNodes.size();
			node.GetNextStates(candidateCheck, addCandidate);
			for (int i = 0; i < searchNodes.size() - ps; i++)
				depthStack.push(d - 1);
		}
		for (Node* n : nodeList)
			delete n;
	}
	return std::vector<Direction>();
}

bool IDDepthFirstSearch::IsNodeVisited(Node* node)
{
	return m_VisitedNodes.find(node->state) != m_VisitedNodes.end();
}