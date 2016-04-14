#pragma once

#include <Common/Common.h>
#include "Direction.h"

struct Node
{
	int* state;
	uint width, height;
	const Node* parent;
	Vector2i position;
	Direction direction;

	int fCost, gCost, hCost;

	Node(int* state, uint width, uint height, const Node* parent);
	~Node();
	int GetNextDirections();
	std::vector<Node*> GetNextStates();

	void SetCost(int gCost, int hCost);

	inline bool operator==(const Node& other) const
	{
		return memcmp(state, other.state, width * height * sizeof(int)) == 0;
	}

	inline bool operator!=(const Node& other)
	{
		return !(*this == other);
	}
};