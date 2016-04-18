#include "String.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

typedef unsigned int uint;

String ReadStringFromFile(const String& path)
{
	FILE* file = fopen(path.c_str(), "rb");
	if (!file)
	{
		std::cout << "Error: could not open file '" << path << "'" << std::endl;
		__debugbreak();
		return String();
	}
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);

	String result(length, 0);
	fseek(file, 0, SEEK_SET);
	fread(&result[0], 1, length, file);
	fclose(file);

	// Strip carriage returns
	result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
	return result;
}

StringList ReadLinesFromFile(const String& path)
{
	StringList result;
	String string = ReadStringFromFile(path);
	if (string.empty())
		return result;

	return GetLines(string);
}

void WriteStringToFile(const String& string, const String& path)
{
	FILE* file = fopen(path.c_str(), "wt");
	if (!file)
		return;
	fputs(string.c_str(), file);
	fclose(file);
}

StringList SplitString(const String& string, const String& delimiters)
{
	size_t start = 0;
	size_t end = string.find_first_of(delimiters);

	StringList result;

	while (end <= String::npos)
	{
		String token = string.substr(start, end - start);
		if (!token.empty())
			result.push_back(token);

		if (end == String::npos)
			break;

		start = end + 1;
		end = string.find_first_of(delimiters, start);
	}

	return result;
}

StringList SplitString(const String& string, const char delimiter)
{
	return SplitString(string, String(1, delimiter));
}

StringList Tokenize(const String& string)
{
	return SplitString(string, " \t\n");
}

StringList GetLines(const String& string)
{
	return SplitString(string, "\n");
}

const char* FindToken(const char* str, const String& token)
{
	const char* t = str;
	while (t = strstr(t, token.c_str()))
	{
		bool left = str == t || isspace(t[-1]);
		bool right = !t[token.size()] || isspace(t[token.size()]);
		if (left && right)
			return t;

		t += token.size();
	}
	return nullptr;
}

const char* FindToken(const String& string, const String& token)
{
	return FindToken(string.c_str(), token);
}

String GetBlock(const char* str, const char** outPosition)
{
	const char* end = strstr(str, "}");
	if (!end)
		return String(str);

	if (outPosition)
		*outPosition = end;
	int length = end - str + 1;
	return String(str, length);
}

String GetBlock(const String& string, int offset)
{
	const char* str = string.c_str() + offset;
	return GetBlock(str);
}

String GetStatement(const char* str, const char** outPosition)
{
	const char* end = strstr(str, ";");
	if (!end)
		return String(str);

	if (outPosition)
		*outPosition = end;
	int length = end - str + 1;
	return String(str, length);
}

bool StartsWith(const String& string, const String& start)
{
	return string.find(start) == 0;
}

int NextInt(const String& string)
{
	const char* str = string.c_str();
	for (uint i = 0; i < string.size(); i++)
	{
		if (isdigit(string[i]))
		{
			int index = i;
			if (i > 0 && string[i - 1] == '-')
				index--;
			return atoi(&string[index]);
		}
	}
	return -1;
}