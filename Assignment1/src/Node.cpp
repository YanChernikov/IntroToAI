#include "Node.h"
#include "SearchMethod.h"

Node::Node(int* state, uint width, uint height, const Node* parent)
	: parent(parent), width(width), height(height), direction(Direction::NONE), position(0, 0),
		fCost(0), gCost(0), hCost(0)
{
	this->state = new int[width * height];
	memcpy(this->state, state, width * height * sizeof(int));
}

Node::~Node()
{
	delete[] state;
}

int Node::GetNextDirections()
{
	int result = 0;
	if (position.x > 0)
		result |= (int)Direction::LEFT;
	if (position.x < width - 1)
		result |= (int)Direction::RIGHT;
	if (position.y > 0)
		result |= (int)Direction::UP;
	if (position.y < height - 1)
		result |= (int)Direction::DOWN;
	return result;
}

std::vector<Node*> Node::GetNextStates()
{
	std::vector<Node*> results;
	int direction = GetNextDirections();
	int current = (int)Direction::UP;
	while (current < direction)
	{
		if ((direction & current) == 0)
		{
			current <<= 1;
			continue;
		}

		Vector2i next = position;
		switch ((Direction)current)
		{
			case Direction::UP:
				next.y--;
				break;
			case Direction::DOWN:
				next.y++;
				break;
			case Direction::LEFT:
				next.x--;
				break;
			case Direction::RIGHT:
				next.x++;
				break;
		}
		Node* candidate = new Node(state, width, height, this);
		memcpy(candidate->state, state, width * height * sizeof(int));
		candidate->state[position.x + position.y * width] = state[next.x + next.y * width];
		candidate->state[next.x + next.y * width] = state[position.x + position.y * width];
		candidate->direction = (Direction)current;
		candidate->position = next;
		candidate->SetCost(candidate->gCost + 1, candidate->hCost);
		results.push_back(candidate);
		current <<= 1;
	}
	return results;
}

void Node::SetCost(int gCost, int hCost)
{
	fCost = gCost + hCost;
	this->gCost = gCost;
	this->hCost = hCost;
}