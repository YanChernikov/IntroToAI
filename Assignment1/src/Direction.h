#pragma once

#include <Common/Common.h>

enum class Direction : byte
{
	NONE = 0,
	UP = BIT(0),
	DOWN = BIT(1),
	LEFT = BIT(2),
	RIGHT = BIT(3),
};

static String DirectionToString(Direction direction)
{
	switch (direction)
	{
		case Direction::NONE:	return "None";
		case Direction::UP:		return "Up";
		case Direction::DOWN:	return "Down";
		case Direction::LEFT:	return "Left";
		case Direction::RIGHT:	return "Right";
	}
	return "Unknown";
}
