#include "Random.h"

#include <time.h>

Random::Generator Random::s_Generator;

void Random::Init()
{
	s_Generator.seed(time(NULL));
}

int Random::Int()
{
	std::uniform_int_distribution<uint32_t> distribution;
	return distribution(s_Generator);
}

int Random::Int(int max)
{
	std::uniform_int_distribution<uint32_t> distribution(0, max);
	return distribution(s_Generator);
}

int Random::Int(int min, int max)
{
	std::uniform_int_distribution<uint32_t> distribution(min, max);
	return distribution(s_Generator);
}
