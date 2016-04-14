#pragma once

#include <Common/Common.h>
#include "Direction.h"

struct Node
{
	byte* state;
	byte width, height;
	const Node* parent;
	Vector2i position;
	Direction direction;

	short fCost, gCost, hCost;

	Node(byte* state, byte width, byte height, const Node* parent);
	~Node();
	byte GetNextDirections();
	std::vector<Node*> GetNextStates();

	void SetCost(short gCost, short hCost);

	inline bool operator==(const Node& other) const
	{
		return memcmp(state, other.state, width * height * sizeof(byte)) == 0;
	}

	inline bool operator!=(const Node& other)
	{
		return !(*this == other);
	}
};