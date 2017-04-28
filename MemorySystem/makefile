makefile:
all: MemorySystem

MemorySystem: memorysystem.o
	g++ -g -o Debug/MemorySystem memorysystem.o
	
memorysystem.o: memorysystem.cpp memorysystem.h
	g++ -c -g memorysystem.cpp