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
	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);
	int blankTileIndex = puzzle.FindBlankTile();
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);
	byte* goalState = puzzle.GetGoalState()->GetValues();

	struct Stack
	{
		Node* node;
		int depth;
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

	for (int searchDepth = 0; ; searchDepth++)
	{
		int currentDepth = searchDepth;
		m_VisitedNodes.clear();
		nodeList.clear();
		std::stack<Stack> stack;
		stack.push({ root, currentDepth });
		std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);

		while (stack.size() > 0)
		{
			m_Iterations++;
			Stack s = stack.top();
			stack.pop();

			if (s.depth == 0 && IsSolved(s.node->GetState().GetValues(), goalState, width * height))
				return TracePath(*s.node);

			if (s.depth <= 0)
				continue;

			candidates.clear();
			s.node->GetNextStates(candidateCheck, addCandidate);
			for (Node* candidate : candidates)
				stack.push({ candidate, s.depth - 1 });
		}
	}
	return std::vector<Direction>();
}