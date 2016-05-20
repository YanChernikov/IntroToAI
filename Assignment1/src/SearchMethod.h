#pragma once

#include <Common/Common.h>
#include "Node.h"
#include "Direction.h"

#include <unordered_set>

class Puzzle;
struct StatePtr
{
	const State* state;
	StatePtr(const State& s) : state(&s)
	{

	}
	bool operator==(const StatePtr& other) const
	{
		return *state == *other.state;
	}
};

namespace std
{
	template <> struct hash<StatePtr>
	{
		size_t operator()(const StatePtr& s) const
		{
			hash<State> h;
			return h(*s.state);
		}
	};
}

class SearchMethod
{
protected:
	std::unordered_set<StatePtr> m_VisitedNodes;
	int m_Iterations;
public:
	SearchMethod()
		: m_Iterations(0)
	{
	}

	virtual ~SearchMethod()
	{
	}

	virtual std::vector<Direction> Solve(const Puzzle& puzzle) = 0;

	inline bool IsSolved(byte* state, byte* goal, byte size)
	{
		return memcmp(state, goal, size * sizeof(byte)) == 0;
	}

	bool IsStateVisited(State& state)
	{
		return m_VisitedNodes.find(state) != m_VisitedNodes.end();
	}


	bool SetContains(const std::vector<Node*>& set, const State& state) const
	{
		for (Node* n : set)
		{
			if (n->GetState() == state)
				return true;
		}
		return false;
	}

	virtual String GetName() = 0;
	inline int GetIterationCount() const { return m_Iterations; }
};
