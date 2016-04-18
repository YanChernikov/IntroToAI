#pragma once

#include "Common/Common.h"
#include "Node.h"
#include "SearchMethod.h"

class Puzzle
{
private:
	byte* m_State;
	byte* m_GoalState;
	byte m_Width, m_Height;
public:
	Puzzle(byte width, byte height);
	~Puzzle();

	void SetCurrentState(const String& state);
	void SetGoalState(const String& state);

	int FindBlankTile() const;
	std::vector<Direction> Solve(SearchMethod& method, bool wow);

	inline byte* GetState() const { return m_State; }
	inline byte* GetGoalState() const { return m_GoalState; }

	inline byte GetWidth() const { return m_Width; }
	inline byte GetHeight() const { return m_Height; }
};
