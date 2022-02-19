#include <iostream>
#include <fstream>
#include "Parser.h"

using namespace LaTeX::PRSR;

int main() {// ()
	std::string title;
	std::cin >> title;
	Content main = DefaultEnvironment({
		SimpleDefaultCommand("documentclaass","article"),
		NamedDefaultEnvironment("document",{
			SimpleDefaultCommand("title",title)
		})
	});
	std::ofstream file;
	file.open("newfile.tex", std::fstream::out);
	file << main.toString();
	std::cout << main.toString();
	return 0;
}