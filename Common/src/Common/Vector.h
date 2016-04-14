#pragma once

struct Vector2i
{
	int x, y;

	inline Vector2i()
		: x(0), y(0)
	{
	}

	inline Vector2i(const Vector2i& other)
		: x(other.x), y(other.y)
	{
	}

	inline Vector2i(int x, int y)
		: x(x), y(y)
	{
	}


};
