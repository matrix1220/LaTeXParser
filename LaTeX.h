#pragma once
#include "includes.h"

namespace LaTeX
{

// class Stream: public std::vector<T>
class Stream: public std::deque<char>
{
public:
	Stream();
};

bool operator>>(Stream&R, char&L);
void operator>>(char&R, Stream&L);
void operator<<(Stream&R, char&L);
bool operator<<(char&R, Stream&L);

void readFileToStream(std::string filename, Stream& stream);


}
