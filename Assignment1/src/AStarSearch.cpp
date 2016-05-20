#include "AStarSearch.h"
#include "Puzzle.h"

AStarSearch::AStarSearch()
{
}

// Traces the path from the goal node to the start, adding each direction into the resulting vector
std::vector<Direction> AStarSearch::TracePath(const Node& leaf)
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

std::vector<Direction> AStarSearch::Solve(const Puzzle& puzzle)
{
	// Retrieve our puzzle parameters
	int blankTileIndex = puzzle.FindBlankTile();
	byte* goalState = puzzle.GetGoalState()->GetValues();
	byte width = puzzle.GetWidth();
	byte height = puzzle.GetHeight();

	Node* root = Node::Create((State*)puzzle.GetState(), width, height, nullptr);
	root->SetCost(0, CalculateHeuristic(root->GetState().GetValues(), goalState, width, height));
	root->position = Vector2i(blankTileIndex % width, blankTileIndex / width);

	// Store every node we allocate as a unique pointer so that we can delete all of them when we exit this function
	std::vector<std::unique_ptr<Node>> nodeList;
	std::vector<Node*> candidates;
	std::vector<Node*> closedSet, openSet;
	std::function<bool(State&)> candidateCheck = [&](State& s)
	{
		// Check to make sure the node we are considering isn't in the closedSet
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
		// Find best node (by cost) to expand
		Node* current = FindBestNode(openSet, &index);
		if (IsSolved(current->GetState().GetValues(), goalState, width * height))
			return TracePath(*current);

		openSet.erase(openSet.begin() + index);
		closedSet.push_back(current);
		current->GetNextStates(candidateCheck, addCandidate);
		for (Node* candidate : candidates)
		{
			// Add a movement cost of 1 to advance to the next node
			int gCost = current->gCost + 1;
			// Add the node to the openSet if it doesn't already contain it
			if (!SetContains(openSet, candidate->GetState()))
				openSet.push_back(candidate);
			else if (gCost >= candidate->gCost)
				continue;

			// Set the cost of our candidate
			candidate->SetCost(gCost, CalculateHeuristic(candidate->GetState().GetValues(), goalState, width, height));
		}
		candidates.clear();
	}
	return std::vector<Direction>();
}

// This function finds the node with the lowest fCost in the given vector,
// and optionally provides its index in that vector
Node* AStarSearch::FindBestNode(const std::vector<Node*>& set, int* index)
{
	Node* result = nullptr;
	// Initialize min with some large value
	int min = 100000;
	for (uint i = 0; i < set.size(); i++)
	{
		Node* node = set[i];
		if (node->fCost < min)
		{
			min = node->fCost;
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
short AStarSearch::CalculateHeuristic(byte* state, byte* goalState, byte width, byte height)
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