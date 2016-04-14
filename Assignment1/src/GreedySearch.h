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
	inline static String GetStaticName() { return "Greedy Search"; }
	inline static String GetCode() { return "GS"; }
	inline String GetName() override { return GetStaticName(); }
private:
	Node* FindBestNode(const std::vector<Node*>& set, int* index = nullptr);
	bool SetContains(const std::vector<Node*>& set, Node* node);
	short CalculateHeuristic(byte* state, byte* goalState, byte width, byte height);
};
