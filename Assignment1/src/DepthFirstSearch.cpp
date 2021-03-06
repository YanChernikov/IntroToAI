#include "DepthFirstSearch.h"
#include "Puzzle.h"

#include <stack>

DepthFirstSearch::DepthFirstSearch()
{
}

// Traces the path from the goal node to the start, adding each direction into the resulting vector
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
	byte* goalState = puzzle.GetGoalState()->GetValues();

	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);
	int blankTileIndex = puzzle.FindBlankTile(); // Find the index of the blank tile
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width); // Calculate a 2D position from the blank tile's index

	// Store every node we allocate as a unique pointer so that we can delete all of them when we exit this function
	std::vector<std::unique_ptr<Node>> nodeList;
	nodeList.push_back(std::unique_ptr<Node>(root));

	std::stack<Node*> searchNodes;
	searchNodes.push(root);

	// Create two functions passed into the Node::GetNextStates() method to define how child nodes get evaluated
	// and what should happen if they are determined to be valid
	std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
	std::function<void(Node*)> addCandidate = [&](Node* n)
	{
		m_VisitedNodes.insert(n->GetState());
		searchNodes.push(n);
		nodeList.push_back(std::unique_ptr<Node>(n));
	};

	// Start the non-recursive depth-first search algorithm
	while (searchNodes.size() > 0)
	{
		m_Iterations++;
		Node& node = *searchNodes.top();
		searchNodes.pop();

		// Return solution if solved
		if (IsSolved(node.GetState().GetValues(), goalState, width * height))
			return TracePath(node);

		node.GetNextStates(candidateCheck, addCandidate);
	}
	return std::vector<Direction>();
}