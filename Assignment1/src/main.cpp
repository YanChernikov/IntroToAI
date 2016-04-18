#include <iostream>

#include "Common/Common.h"
#include "Puzzle.h"
#include "BreadthFirstSearch.h"
#include "DepthFirstSearch.h"
#include "AStarSearch.h"
#include "GreedySearch.h"

// TODO: Replace assertions with errors

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		std::cout << "Incorrect input arguments!" << std::endl;
		std::cout << "\tUsage: search filename method(BFS|DFS|AS) [output_filename]" << std::endl;
		return 1;
	}
	
	String filepath = argv[1];
	String method = argv[2];
	StringList lines = ReadLinesFromFile(filepath);
	ASSERT(lines.size() >= 3);
	StringList puzzleSize = ::SplitString(lines[0], "x");
	ASSERT(puzzleSize.size() == 2);
	int width = NextInt(puzzleSize[0]);
	int height = NextInt(puzzleSize[1]);

	Puzzle puzzle(width, height);
	puzzle.SetCurrentState(lines[1]);
	puzzle.SetGoalState(lines[2]);

	SearchMethod* smptr = nullptr;
	if (method == BreadthFirstSearch::GetCode())
		smptr = new BreadthFirstSearch();
	else if (method == DepthFirstSearch::GetCode())
		smptr = new DepthFirstSearch();
	else if (method == AStarSearch::GetCode())
		smptr = new AStarSearch();
	else if (method == GreedySearch::GetCode())
		smptr = new GreedySearch();

	ASSERT(smptr);

	SearchMethod& search = *smptr;
	std::vector<Direction> result;
	float time;
	Timer timer;
	for (int i = 0; i < 1; i++)
	{
		result = puzzle.Solve(search, i == 0);
	}
	time = timer.ElapsedMillis();

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
	}

	if (argc > 3)
	{
		String fileout = argv[3];
		String output;
		for (auto dir : result)
			output += DirectionToString(dir) + "\n";
		WriteStringToFile(output, fileout);
	}

	system("PAUSE");
	delete smptr;
	return 0;
}