#include <iostream>
#include "Parser.h"

int main() {
	try {
		LaTeX::Stream stream;
		LaTeX::readFileToStream("sample.tex", stream);
		LaTeX::TKNZR::Tokenizer tokenizer(stream);

		LaTeX::PRSR::Content result = LaTeX::PRSR::Parse(tokenizer);
		std::cout << result.toString();
	} catch (std::exception &e) {
		std::cout << e.what();
	}
	return 0;
}