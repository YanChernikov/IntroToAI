#pragma once

#include <Common/Common.h>
#include "Direction.h"

struct State
{
	static const int c_Size = 25;
	byte values[c_Size];
	
	State()
	{
		memset(this, 0, c_Size);
	}
	inline bool operator==(const State& other) const
	{
		return memcmp(values, other.values, c_Size) == 0;
	}
};

namespace std
{
	template<> struct hash<State>
	{
		size_t operator()(const State& s) const
		{
			size_t h = 2166136261;
			for (int i = 0; i < State::c_Size; i++)
			{
				h = h * 16777619;
				h ^= s.values[i];
			}
			return h;
		}
	};
}

struct Node
{
	State state;
	byte width, height;
	const Node* parent;
	Vector2i position;
	Direction direction;

	short fCost, gCost, hCost;

	Node(State* state, byte width, byte height, const Node* parent);
	~Node();
	byte GetNextDirections();
	void GetNextStates(const std::function<bool(State&)>& check, const std::function<void(Node*)>& accept);

	void SetCost(short gCost, short hCost);

	inline bool operator==(const Node& other) const
	{
		return state == other.state;
	}

	inline bool operator!=(const Node& other)
	{
		return !(*this == other);
	}

/*	static void* s_freelist;
	inline void* operator new(size_t s)
	{
		if (!s_freelist)
		{
			unsigned char* blob = (unsigned char*)malloc(sizeof(Node) * 128);
			*(void**)&blob[127 * sizeof(Node)] = nullptr;
			for (int i = 126; i >= 0; i--)
			{
				void* vp = &blob[i * sizeof(Node)];
				*(void**)vp = &blob[(i + 1) * sizeof(Node)];
			}
			s_freelist = blob;
		}
		void* r = s_freelist;
		s_freelist = *(void**)s_freelist;
		return r;
	}

	inline void operator delete(void* p)
	{
		*(void**)p = s_freelist;
		s_freelist = p;
	}*/

};