#pragma once

#include "Common/String.h"

struct Route
{
	String origin, destination;
	int distance, directDistance;

	String ToString() const;
};
