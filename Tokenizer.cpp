#include "Tokenizer.h"

namespace LaTeX
{
namespace TKNZR
{

Content::Content() {}
Content::Content(Type type, std::string content): type(type), content(content) {}
void Content::operator=(const Content& temp) {
	type = temp.type;
	content = temp.content;
}
std::string Content::toString(int i) const {
	std::string result;//(i, ' ');
	if(type==Type::Text or type==Type::Token or type==Type::Symbol) {
		result+= content;
	} else if(type==Type::Command) {
		result+='\\'+ content;
	}
	return result;
}

Tokenizer::Tokenizer(Stream& stream): stream(stream) {}
bool Tokenizer::operator>>(Content& rightObject) {
	produce = true;
	while (produce) {
		if(!(stream >> current)) {
			if(checkIfBlockEnds()) {
				blockEnds(rightObject);
				return true;
			} else {
				return false;
			}
		}
		//std::cout << current;

		if(status==Status::READING_COMMENT) {
			if(current=='\n') status=Status::READY_TO_READ;
		} else if(status==Status::READY_TO_COMMAND and (
				current=='\\' or
				current==' ' or current=='\t' or current=='\n' or
				current=='%' or
				checkIfToken()
			)) {
			recordSymbol(rightObject);
		} else if(current=='\\') {
			if(checkIfBlockEnds()) {
				stream << current;
				blockEnds(rightObject);
			} else {
				status = Status::READY_TO_COMMAND;
			}
		} else if(current==' ' or current=='\t' or current=='\n') {
			if(checkIfBlockEnds()) {
				stream << current;
				blockEnds(rightObject);
			}
		} else if(checkIfToken()) {
			if(checkIfBlockEnds()) {
				stream << current;
				blockEnds(rightObject);
			} else {
				recordToken(rightObject);
			}
		} else if(current=='%') {
			if(checkIfBlockEnds()) {
				stream << current;
				blockEnds(rightObject);
			} else {
				status = Status::READING_COMMENT;
			}
		} else {
			if(status==Status::READY_TO_READ) status = Status::READING_TEXT;
			else if(status==Status::READY_TO_COMMAND) status = Status::READING_COMMAND;
			//std::cout << current;
			buffer += current;
		}
	}
	return true;
}
bool Tokenizer::checkIfToken() {
	if( current=='[' or current==']' or current=='{' or current=='}' or
		current==',' or current=='.' or current=='$' or current==':' or
		current=='(' or current==')' or current=='^' or current=='=' or
		current==';' or current=='-' or current=='*' or current=='/' or
		current=='<' or current=='>' or current=='&' or current=='+' or 
		current=='_' or current=='"' or current=='\'' ) return true;
		return false;
}
bool Tokenizer::checkIfBlockEnds() {
	return status==Status::READING_TEXT or status==Status::READING_COMMAND;
}
void Tokenizer::blockEnds(Content& rightObject) {
	produce = false;
	if(status==Status::READING_TEXT) {  // move
		rightObject = Content(Type::Text, buffer);
	} else if(status==Status::READING_COMMAND) {
		rightObject = Content(Type::Command, buffer);
	}
	buffer.clear();
	status=Status::READY_TO_READ;
}
void Tokenizer::recordToken(Content& rightObject) {
	produce = false;
	rightObject = Content(Type::Token, std::string(1, current));
	status=Status::READY_TO_READ;
}
void Tokenizer::recordSymbol(Content& rightObject) {
	produce = false;
	rightObject = Content(Type::Symbol, std::string(1, current));
	status=Status::READY_TO_READ;
}

}
}