#include <iostream>

#include "Common/String.h"
#include "Route.h"

int main(int argc, char* argv[])
{
	std::vector<Route> routes;

	StringList lines = ReadLinesFromFile("res/Example.txt");
	for (const String& line : lines)
	{
		StringList tokens = Tokenize(line);
		if (tokens.size() != 4)
			continue;

		routes.push_back({ tokens[0], tokens[1], NextInt(tokens[2]), NextInt(tokens[3]) });
	}	

	std::cout << "Routes:" << std::endl;
	for (const Route& route : routes)
	{
		if (route.distance > 0)
			std::cout << "Travel from " << route.origin << " to " << route.destination << " with a straight line distance of " << route.directDistance << " and an actual distance of " << route.distance << std::endl;
		else
			std::cout << "Cannot drive from " << route.origin << " to " << route.destination << ", however there is a straight line distance of " << route.directDistance << "." << std::endl;
	}

	system("PAUSE");
	return 0;
}