#pragma once

#include <random>

class Random
{
private:
	typedef std::mt19937 Generator;
private:
	static Generator s_Generator;
public:
	static void Init();
	static int Int();
	static int Int(int max);
	static int Int(int min, int max);
};
