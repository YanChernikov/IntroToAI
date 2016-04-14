#pragma once

#include <thread>

class Thread
{
public:
	static inline void Sleep(int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}
};
