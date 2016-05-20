#include "BreadthFirstSearch.h"
#include "Puzzle.h"

#include <queue>

BreadthFirstSearch::BreadthFirstSearch()
{
}

// Traces the path from the goal node to the start, adding each direction into the resulting vector
std::vector<Direction> BreadthFirstSearch::TracePath(const Node& leaf)
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

std::vector<Direction> BreadthFirstSearch::Solve(const Puzzle& puzzle)
{
	m_VisitedNodes.clear();
	m_Puzzle = &puzzle;

	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();
	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);

	// Store every node we allocate as a unique pointer so that we can delete all of them when we exit this function
	std::vector<std::unique_ptr<Node>> nodeList;
	nodeList.push_back(std::unique_ptr<Node>(root));
	int blankTileIndex = puzzle.FindBlankTile();
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);
	std::queue<Node*> searchNodes;
	searchNodes.push(root);
	byte* goalState = puzzle.GetGoalState()->GetValues();

	// Create two functions passed into the Node::GetNextStates() method to define how child nodes get evaluated
	// and what should happen if they are determined to be valid
	std::function<bool(State&)> candidateCheck = std::bind(&SearchMethod::IsStateVisited, this, std::placeholders::_1);
	std::function<void(Node*)> addCandidate = [&](Node* n)
	{
		m_VisitedNodes.insert(n->GetState());
		searchNodes.push(n);
		nodeList.push_back(std::unique_ptr<Node>(n));
	};

	while (searchNodes.size() > 0)
	{
		m_Iterations++;
		Node& node = *searchNodes.front();
		searchNodes.pop();

		if (IsSolved(node.GetState().GetValues(), goalState, width * height))
			return TracePath(node);

		node.GetNextStates(candidateCheck, addCandidate);
	}
	return std::vector<Direction>();
}