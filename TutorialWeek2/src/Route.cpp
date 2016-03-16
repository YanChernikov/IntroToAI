#include "Route.h"

String Route::ToString() const
{
	String result;
	result += "Route: \n";
	result += "\t" + origin + " -> " + destination + "\n";

	String distanceString = distance < 0 ? "No route found!" : std::to_string(distance);
	result += "\tDistance: " + distanceString + "\n";
	result += "\tDirect Distance: " + std::to_string(directDistance) + "\n";
	return result;
}