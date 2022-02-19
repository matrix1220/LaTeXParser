#include "LaTeX.h"
#include "Tokenizer.h"
#include "Parser.h"
namespace LaTeX
{

Stream::Stream(){}

bool operator>>(Stream&R, char&L) {
	if(R.empty()) {
		return false;
	} else {
		L = R.back();
		R.pop_back();
		return true;
	}
}
void operator>>(char&R, Stream&L) {
	L.push_front(R);
}
void operator<<(Stream&R, char&L) {
	R.push_back(L);
}
bool operator<<(char&R, Stream&L) {
	if(L.empty()) {
		return false;
	} else {
		R = L.front();
		L.pop_front();
		return true;
	}
}
void readFileToStream(std::string filename, Stream& stream) {
	std::ifstream file;
	file.open(filename, std::ifstream::in);
	char current;
	while(file.get(current)) {
		current >> stream;
	}
}


}