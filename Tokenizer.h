#pragma once
#include "includes.h"
#include "LaTeX.h"

namespace LaTeX
{
namespace TKNZR
{

enum class Status
{
	READY_TO_READ,		//= 1 << 0,
	READY_TO_COMMAND,	//= 1 << 1,
	READING_COMMENT,	//= 1 << 2,
	READING_TEXT,		//= 1 << 3,
	READING_COMMAND 	//= 1 << 4
};

enum class Type
{
	Text,
	Token,
	Command,
	Symbol
};


class Content {
public:
	Type type;
	std::string content;
	Content();
	Content(Type type, std::string content);
	void operator=(const Content& temp);
	std::string toString(int i=0) const;
};

class Tokenizer
{
	Stream& stream;
	Status status = Status::READY_TO_READ;

	std::string buffer;
	char current;
	bool produce;
public:
	Tokenizer(Stream& stream);
	bool operator>>(Content& rightObject);
	bool checkIfToken();
	inline bool checkIfBlockEnds();
	void blockEnds(Content& rightObject);
	void recordToken(Content& rightObject);
	void recordSymbol(Content& rightObject);
	
};


}
}