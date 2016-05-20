#pragma once

#include <string>
#include <vector>

// A series of string manipulation functions that proved useful for parsing
// the input text file

typedef std::string String;
typedef std::vector<String> StringList;

String ReadStringFromFile(const String& path);
StringList ReadLinesFromFile(const String& path);

void WriteStringToFile(const String& string, const String& path);

StringList SplitString(const String& string, const String& delimiters);
StringList SplitString(const String& string, const char delimiter);
StringList Tokenize(const String& string);
StringList GetLines(const String& string);

const char* FindToken(const char* str, const String& token);
const char* FindToken(const String& string, const String& token);

String GetBlock(const char* str, const char** outPosition = nullptr);
String GetBlock(const String& string, int offset = 0);

String GetStatement(const char* str, const char** outPosition = nullptr);

bool StartsWith(const String& string, const String& start);
int NextInt(const String& string);
