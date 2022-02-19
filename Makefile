# make -f Makefile-2 clean

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -fconcepts -std=c++17

all: libLaTeX.a

libLaTeX.a: Parser.o Tokenizer.o LaTeX.o
	ar rcs -o libLaTeX.a Parser.o Tokenizer.o LaTeX.o


Parser.o: Parser.cpp Parser.h
	$(CC) $(COMPILER_FLAGS) -c Parser.cpp

Tokenizer.o: Tokenizer.cpp Tokenizer.h
	$(CC) $(COMPILER_FLAGS) -c Tokenizer.cpp

LaTeX.o: LaTeX.cpp LaTeX.h
	$(CC) $(COMPILER_FLAGS) -c LaTeX.cpp