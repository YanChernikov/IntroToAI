#pragma once

#include "Common/Common.h"
#include "Node.h"
#include "SearchMethod.h"

class Puzzle
{
private:
	int* m_State;
	int* m_GoalState;
	uint m_Width, m_Height;
public:
	Puzzle(uint width, uint height);
	~Puzzle();

	void SetCurrentState(const String& state);
	void SetGoalState(const String& state);

	int FindBlankTile() const;
	std::vector<Direction> Solve(SearchMethod& method);

	inline int* GetState() const { return m_State; }
	inline int* GetGoalState() const { return m_GoalState; }

	inline uint GetWidth() const { return m_Width; }
	inline uint GetHeight() const { return m_Height; }
};
