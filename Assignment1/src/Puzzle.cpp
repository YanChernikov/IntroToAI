#include "Puzzle.h"

Puzzle::Puzzle(byte width, byte height)
	: m_Width(width), m_Height(height)
{
	// Create the initial and goal states
	m_State = State::Create(width, height);
	m_GoalState = State::Create(width, height);
}

Puzzle::~Puzzle()
{
	free(m_State);
	free(m_GoalState);
}

// Parses the given state string and sets it as the initial state
void Puzzle::SetCurrentState(const String& state)
{
	StringList tokens = Tokenize(state);
	ASSERT(tokens.size() == m_Width * m_Height);
	for (uint i = 0; i < tokens.size(); i++)
		m_State->GetValues()[i] = NextInt(tokens[i]);
}

// Parses the given state string and sets it as the goal state
void Puzzle::SetGoalState(const String& state)
{
	StringList tokens = Tokenize(state);
	ASSERT(tokens.size() == m_Width * m_Height);
	for (uint i = 0; i < tokens.size(); i++)
		m_GoalState->GetValues()[i] = NextInt(tokens[i]);
}

// Finds the index of the blank tile (the tile set to 0)
int Puzzle::FindBlankTile() const
{
	for (int i = 0; i < m_Width * m_Height; i++)
	{
		if (m_State->GetValues()[i] == 0)
			return i;
	}
	return -1;
}

// Attempts to solve the puzzle by running the provided search method
std::vector<Direction> Puzzle::Solve(SearchMethod& method)
{
	std::cout << "Attempting to solve puzzle using " << method.GetName() << std::endl;
	std::cout << "Solving..." << std::endl;
	return method.Solve(*this);
}
