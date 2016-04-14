#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
public:
	inline Timer()
	{
		Reset();
	}

	inline void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	inline float Elapsed()
	{
		return ElapsedMillis() / 1000.0f;
	}

	inline float ElapsedMillis()
	{
		return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::high_resolution_clock::now() - m_Start).count();
	}
};