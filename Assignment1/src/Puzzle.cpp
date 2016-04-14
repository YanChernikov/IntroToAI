#include "Puzzle.h"

Puzzle::Puzzle(byte width, byte height)
	: m_Width(width), m_Height(height)
{
	m_State = new byte[width * height];
	m_GoalState = new byte[width * height];
}

Puzzle::~Puzzle()
{
	delete[] m_State;
	delete[] m_GoalState;
}

void Puzzle::SetCurrentState(const String& state)
{
	StringList tokens = Tokenize(state);
	ASSERT(tokens.size() == m_Width * m_Height);
	for (int i = 0; i < tokens.size(); i++)
		m_State[i] = NextInt(tokens[i]);
}

void Puzzle::SetGoalState(const String& state)
{
	StringList tokens = Tokenize(state);
	ASSERT(tokens.size() == m_Width * m_Height);
	for (int i = 0; i < tokens.size(); i++)
		m_GoalState[i] = NextInt(tokens[i]);
}

int Puzzle::FindBlankTile() const
{
	for (int i = 0; i < m_Width * m_Height; i++)
	{
		if (m_State[i] == 0)
			return i;
	}
	return -1;
}

std::vector<Direction> Puzzle::Solve(SearchMethod& method)
{
	std::cout << "Attempting to solve puzzle using " << method.GetName() << std::endl;
	std::cout << "Solving..." << std::endl;
	return method.Solve(*this);
}
