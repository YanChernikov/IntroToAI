#pragma once

#include <Common/Common.h>
#include "Direction.h"

// State struct holds the puzzle state as an array of bytes: one for each tile
struct State
{
	friend struct Node; // So that Node can access our constructor

	// The size of our puzzle (eg. 3x3 = 9)
	short size;

	inline bool operator==(const State& other) const
	{
		return memcmp(GetValues(), other.GetValues(), size) == 0;
	}

	// Returns the size of our State struct, including the puzzle tiles
	static short Size(byte width, byte height)
	{
		return (short)(sizeof(State) + width * height * sizeof(byte));
	}

	byte* GetValues() const
	{
		// Since we store our tiles at the end of our State struct, this
		// will return a pointer to the beginning of our tile array (which
		// is essentially the location of our State struct + 2 bytes for
		// the size short)
		return (byte*)(this + 1);
	}

	static State* Create(int width, int height)
	{
		// Creates a new State with enough space to store the tile data
		// See the Node::Create for a more detailed explanation
		State* s = (State*)malloc(State::Size(width, height));
		return new(s) State(width * height);
	}

	// Since we use malloc to allocate memory for our State, we should
	// use free to free that memory. The default delete operator will typically
	// call the destructor and then free anyway, however it isn't ~guaranteed~.
	void operator delete(void* ptr)
	{
		free(ptr);
	}

	// Placement deallocation function for the same reason as above
	void operator delete(void* ptr, void* place)
	{
		free(place);
	}
private:
	// Private constructor since State::Create should be used to create States
	State(short size)
		: size(size)
	{
	}
};

namespace std
{
	// Provide hash function for State so that it can be used in unordered_set
	template<>
	struct hash<State>
	{
		size_t operator()(const State& s) const
		{
			// FNV hash function from http://www.isthe.com/chongo/tech/comp/fnv/
			size_t h = 2166136261;
			for (int i = 0; i < s.size; i++)
			{
				h = h * 16777619;
				h ^= s.GetValues()[i];
			}
			return h;
		}
	};
}

struct Node
{
	byte width, height; // Width and height of puzzle
	const Node* parent; // Pointer to this node's parent
	Vector2i position; // Position of blank tile in the puzzle
	Direction direction; // Move direction which led to this node

	// For informed search algorithms
	short fCost, gCost, hCost;

	static Node* Create(State* src, byte width, byte height, const Node* parent);

	~Node();
	byte GetNextDirections();
	void GetNextStates(const std::function<bool(State&)>& check, const std::function<void(Node*)>& accept);

	void SetCost(short gCost, short hCost);

	inline bool operator==(const Node& other) const
	{
		return GetState() == other.GetState();
	}

	inline bool operator!=(const Node& other)
	{
		return !(*this == other);
	}

	// Since we use malloc to allocate memory for our Node (+ State), we should
	// use free to free that memory. The default delete operator will typically
	// call the destructor and then free anyway, however it isn't ~guaranteed~.
	void operator delete(void* ptr)
	{
		free(ptr);
	}

	// Placement deallocation function for the same reason as above
	void operator delete(void* ptr, void* place)
	{
		free(place);
	}

	State& GetState() const
	{
		// This function return a reference to this Node's State. Since we store
		// it at the end of our Node struct, we can just return a dereferenced
		// pointer offset to the next byte after our Node struct, since that
		// will be the beginning of the State data
		return *(State*)(this + 1);
	}
private:
	// Private constructor since we need to allocate enough space for State (via Node::Create)
	Node(State* state, byte width, byte height, const Node* parent);
};