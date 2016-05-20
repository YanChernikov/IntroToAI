#include <iostream>

#include "Common/Common.h"
#include "Puzzle.h"
#include "BreadthFirstSearch.h"
#include "DepthFirstSearch.h"
#include "AStarSearch.h"
#include "GreedySearch.h"
#include "IDDepthFirstSearch.h"
#include "IDAStarSearch.h"

// TODO: Replace assertions with errors

static void PrintUsage()
{
	std::cout << "\tUsage: search filename method(BFS|DFS|GBFS|AS|CUS1|CUS2)" << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		std::cout << "Incorrect input arguments!" << std::endl;
		PrintUsage();
		return 1;
	}
	
	String filepath = argv[1];
	String method = argv[2];
	StringList lines = ReadLinesFromFile(filepath);
	if (lines.size() < 3)
	{
		std::cout << "Invalid input file format!" << std::endl;
		PrintUsage();
		return 2;
	}

	StringList puzzleSize = ::SplitString(lines[0], "x");
	if (puzzleSize.size() != 2)
	{
		std::cout << "Invalid input file format!" << std::endl;
		PrintUsage();
		return 2;
	}

	int width = NextInt(puzzleSize[0]);
	int height = NextInt(puzzleSize[1]);

	Puzzle puzzle(width, height);
	puzzle.SetCurrentState(lines[1]);
	puzzle.SetGoalState(lines[2]);

	// Find the correct search algorithm to use based on the specified code
	SearchMethod* smptr = nullptr;
	if (method == BreadthFirstSearch::GetCode())
		smptr = new BreadthFirstSearch();
	else if (method == DepthFirstSearch::GetCode())
		smptr = new DepthFirstSearch();
	else if (method == AStarSearch::GetCode())
		smptr = new AStarSearch();
	else if (method == GreedySearch::GetCode())
		smptr = new GreedySearch();
	else if (method == IDDepthFirstSearch::GetCode())
		smptr = new IDDepthFirstSearch();
	else if (method == IDAStarSearch::GetCode())
		smptr = new IDAStarSearch();

	if (smptr == nullptr)
	{
		std::cout << "Invalid search method!" << std::endl;
		PrintUsage();
		return 3;
	}

	SearchMethod& search = *smptr;
	std::vector<Direction> result;
	float time = 0.0f;
	{
		// Time the result
		Timer timer;
		result = puzzle.Solve(search);
		time = timer.ElapsedMillis();
	}

	if (result.size() == 0)
	{
		std::cout << "No solution found." << std::endl;
	}
	else
	{
		std::cout << "Done." << std::endl;
		std::cout << "Solution found in ";
		if (time < 2000.0f)
			std::cout << time << "ms";
		else
			std::cout << (time / 1000.0f) << "s";
		std::cout << std::endl;
		std::cout << "Steps: " << result.size() << std::endl;
		std::cout << "Iterations: " << search.GetIterationCount() << std::endl;

		for (Direction direction : result)
			std::cout << DirectionToString(direction) << ";";
		std::cout << std::endl;
	}

	delete smptr;
	return 0;
}