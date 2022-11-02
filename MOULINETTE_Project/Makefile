SYSTEM = x86-64_linux

CCC = g++ -O0
CC  = gcc -O0

all: 
	make all_c
	make all_cpp

execute: all
	make execute_c
	make execute_cpp

PREFIX_PATH = ../../SGBDS_Project

INCLUDES_OFF = $(PREFIX_PATH)/BUSINESS_ENTITIES/*.cpp $(PREFIX_PATH)/HANDLERS/*.cpp $(PREFIX_PATH)/SGBDS_Project/*.cpp $(PREFIX_PATH)/GRAPH_Generators/*.cpp 

livrable1: livrable1
	$(CCC) -g $(INCLUDES_OFF) ./SGBDS_Project.cpp -o livrable1