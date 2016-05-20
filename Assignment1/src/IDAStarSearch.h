#pragma once

#include "SearchMethod.h"

class IDAStarSearch : public SearchMethod
{
private:
	const Puzzle* m_Puzzle;
public:
	IDAStarSearch();

	std::vector<Direction> TracePath(const Node& leaf);
	std::vector<Direction> Solve(const Puzzle& puzzle) override;

	inline int GetIterationCount() const { return m_Iterations; }
	inline static String GetStaticName() { return "Iterative Deepening A* Search"; }
	inline static String GetCode() { return "CUS2"; }
	inline String GetName() override { return GetStaticName(); }
private:
	short CalculateHeuristic(byte* state, byte* goalState, byte width, byte height);
};
