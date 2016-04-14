#pragma once

#include <iostream>
#include "String.h"
#include "Thread.h"
#include "Random.h"
#include "Vector.h"
#include "Timer.h"

typedef unsigned int uint;
typedef unsigned char byte;

#define ASSERT(x) if (!(x)) {\
			__debugbreak(); \
		}

#define BIT(x) 1 << x