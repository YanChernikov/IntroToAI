#pragma once

#include "SearchMethod.h"

class IDDepthFirstSearch : public SearchMethod
{
private:
	const Puzzle* m_Puzzle;
public:
	IDDepthFirstSearch();

	std::vector<Direction> TracePath(const Node& leaf);
	std::vector<Direction> Solve(const Puzzle& puzzle) override;

	inline int GetIterationCount() const { return m_Iterations; }
	inline static String GetStaticName() { return "Iterative Deepening Depth-First Search"; }
	inline static String GetCode() { return "CUS1"; }
	inline String GetName() override { return GetStaticName(); }
};
