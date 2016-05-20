#include "Node.h"
#include "SearchMethod.h"

Node::Node(State* state, byte width, byte height, const Node* parent)
	: parent(parent), width(width), height(height), direction(Direction::NONE), position(0, 0),
		fCost(0), gCost(0), hCost(0)
{
	// Placement new to call State constructor
	new (&GetState()) State(width * height);
	// Copy the supplied state int our allocated state
	memcpy(GetState().GetValues(), state->GetValues(), width * height * sizeof(byte));
}

Node::~Node()
{
}

Node* Node::Create(State* src, byte width, byte height, const Node* parent)
{
	// Allocate enough space for both our Node struct and the puzzle state
	Node* n = (Node*)malloc(sizeof(Node) + State::Size(width, height));
	// Placement new to call Node constructor
	new(n) Node(src, width, height, parent);
	return n;
}

byte Node::GetNextDirections()
{
	// Directions are stored in a single byte, with each bit defining whether a direction is available
	// Eg. 1010 == (Right: yes, left: no, down: yes, up: no), so we can move right or down
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

// Determines all child nodes of this node, calling the accept() function if exclude() returns false
void Node::GetNextStates(const std::function<bool(State&)>& exclude, const std::function<void(Node*)>& accept)
{
	byte direction = GetNextDirections();
	byte current = (byte)Direction::UP;
	State& s = *(new((State*)alloca(State::Size(width, height))) State(width * height));
	// This loop iterates over every direction
	while (current < direction)
	{
		// Is the current iteration's direction a valid direction for us to move in?
		if ((direction & current) == 0)
		{
			current <<= 1; // Advance to the next direction
			continue;
		}

		// Translate direction from our enumeration to an xy delta
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

		// Copy the state into our newly allocated State variable
		memcpy(s.GetValues(), GetState().GetValues(), width * height);

		// Move the empty tile in the specified direction
		s.GetValues()[position.x + position.y * width] = GetState().GetValues()[next.x + next.y * width];
		s.GetValues()[next.x + next.y * width] = GetState().GetValues()[position.x + position.y * width];

		// If the supplied exclusion function does not exclude our candidate node, send it to the accept function
		if (!exclude(s))
		{
			Node* candidate = Node::Create(&s, width, height, this);
			candidate->direction = (Direction)current;
			candidate->position = next;
			// Set the cost for algorithms which require it. Note that this also happens for uninformed search
			// algorithms which don't require this, however it doesn't have a measureable impact on performance,
			// and thus was included here for simplicity's sake
			candidate->SetCost(candidate->gCost + 1, candidate->hCost);
			accept(candidate);
		}
		current <<= 1; // Proceed to check the next direction
	}
}

void Node::SetCost(short gCost, short hCost)
{
	// f(n) = g(n) + h(n)
	// Calculate fCost, which is always a sum of gCost and hCost
	fCost = gCost + hCost;
	this->gCost = gCost;
	this->hCost = hCost;
}