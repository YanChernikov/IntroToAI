#include "Node.h"
#include "SearchMethod.h"

void* Node::s_freelist = nullptr;

Node::Node(State* state, byte width, byte height, const Node* parent)
	: parent(parent), width(width), height(height), direction(Direction::NONE), position(0, 0),
		fCost(0), gCost(0), hCost(0)
{
	RefCount = 0;
	if (parent)
		parent->RefCount++;
	memcpy(this->state.values, state->values, width * height * sizeof(byte));
}

Node::~Node()
{
	if (parent)
	{
		if (--parent->RefCount == 0)
			delete parent;
	}
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

void Node::GetNextStates(const std::function<bool(State&)>& check, const std::function<void(Node*)>& accept)
{
	byte direction = GetNextDirections();
	byte current = (byte)Direction::UP;
	State s;
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
		memcpy(s.values, state.values, State::c_Size);

		s.values[position.x + position.y * width] = state.values[next.x + next.y * width];
		s.values[next.x + next.y * width] = state.values[position.x + position.y * width];

		if (!check(s))
		{
			Node* candidate = new Node(&s, width, height, this);
			candidate->direction = (Direction)current;
			candidate->position = next;
			candidate->SetCost(candidate->gCost + 1, candidate->hCost);
			accept(candidate);
		}
		current <<= 1;
	}
}

void Node::SetCost(short gCost, short hCost)
{
	fCost = gCost + hCost;
	this->gCost = gCost;
	this->hCost = hCost;
}