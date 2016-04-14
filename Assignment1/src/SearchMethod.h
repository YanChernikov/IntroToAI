#pragma once

#include <Common/Common.h>
#include "Node.h"
#include "Direction.h"

class Puzzle;

class SearchMethod
{
protected:
	std::vector<Node*> m_VisitedNodes;
	int m_Iterations;
public:
	SearchMethod()
		: m_Iterations(0)
	{
	}

	virtual ~SearchMethod()
	{
		for (Node* node : m_VisitedNodes)
			delete node;
	}

	virtual std::vector<Direction> Solve(const Puzzle& puzzle) = 0;

	inline bool IsSolved(byte* state, byte* goal, byte size)
	{
		return memcmp(state, goal, size * sizeof(byte)) == 0;
	}

	virtual String GetName() = 0;
	inline int GetIterationCount() const { return m_Iterations; }
};
