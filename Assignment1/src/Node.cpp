#include "Node.h"
#include "SearchMethod.h"

Node::Node(byte* state, byte width, byte height, const Node* parent)
	: parent(parent), width(width), height(height), direction(Direction::NONE), position(0, 0),
		fCost(0), gCost(0), hCost(0)
{
	this->state = new byte[width * height];
	memcpy(this->state, state, width * height * sizeof(byte));
}

Node::~Node()
{
	delete[] state;
}

byte Node::GetNextDirections()
{
	byte result = 0;
	if (position.x > 0)
		result |= (byte)Direction::LEFT;
	if (position.x < width - 1)
		result |= (byte)Direction::RIGHT;
	if (position.y > 0)
		result |= (byte)Direction::UP;
	if (position.y < height - 1)
		result |= (byte)Direction::DOWN;
	return result;
}

std::vector<Node*> Node::GetNextStates()
{
	std::vector<Node*> results;
	byte direction = GetNextDirections();
	byte current = (byte)Direction::UP;
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
		memcpy(candidate->state, state, width * height * sizeof(byte));
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

void Node::SetCost(short gCost, short hCost)
{
	fCost = gCost + hCost;
	this->gCost = gCost;
	this->hCost = hCost;
}