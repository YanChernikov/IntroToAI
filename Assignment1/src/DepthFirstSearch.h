#pragma once

#include "SearchMethod.h"

// Depth-First Search implementation
class DepthFirstSearch : public SearchMethod
{
private:
	const Puzzle* m_Puzzle;
public:
	DepthFirstSearch();
	
	std::vector<Direction> TracePath(const Node& leaf);
	std::vector<Direction> Solve(const Puzzle& puzzle) override;

	inline int GetIterationCount() const { return m_Iterations; }
	inline static String GetStaticName() { return "Depth-First Search"; }
	inline static String GetCode() { return "DFS"; }
	inline String GetName() override { return GetStaticName(); }
};
