#CC specifies which compiler we're using
CC = g++

LIBPATH = B:\Projects\LaTeXParser

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I$(LIBPATH)

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L$(LIBPATH)

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -fconcepts -std=c++17

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lLaTeX

# #OBJS specifies which files to compile as part of the project
# OBJS = ParseFile.cpp

# #OBJ_NAME specifies the name of our exectuable
# OBJ_NAME = ParseFile.exe

#This is the target that compiles our executable
all : ParseFile.exe NewFile.exe

ParseFile.exe : ParseFile.cpp
	$(CC) ParseFile.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ParseFile.exe

NewFile.exe : NewFile.cpp
	$(CC) NewFile.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o NewFile.exe