#pragma once

#include "SearchMethod.h"

class GreedySearch : public SearchMethod
{
private:
	const Puzzle* m_Puzzle;
public:
	GreedySearch();

	std::vector<Direction> TracePath(const Node& leaf);
	std::vector<Direction> Solve(const Puzzle& puzzle) override;

	inline int GetIterationCount() const { return m_Iterations; }
	inline static String GetStaticName() { return "Greedy Best-First Search"; }
	inline static String GetCode() { return "GBFS"; }
	inline String GetName() override { return GetStaticName(); }
private:
	Node* FindBestNode(const std::vector<Node*>& set, int* index = nullptr);
	short CalculateHeuristic(byte* state, byte* goalState, byte width, byte height);
};
