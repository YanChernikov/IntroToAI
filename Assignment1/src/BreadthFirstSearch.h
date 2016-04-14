#pragma once

#include "SearchMethod.h"

class BreadthFirstSearch : public SearchMethod
{
private:
	const Puzzle* m_Puzzle;
public:
	BreadthFirstSearch();
	
	std::vector<Direction> TracePath(const Node& leaf);
	std::vector<Direction> Solve(const Puzzle& puzzle) override;

	inline int GetIterationCount() const { return m_Iterations; }
	inline static String GetStaticName() { return "Breadth-First Search"; }
	inline static String GetCode() { return "BFS"; }
	inline String GetName() override { return GetStaticName(); }
private:
	bool IsNodeVisited(Node* node);
};
