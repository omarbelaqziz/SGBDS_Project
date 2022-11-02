#!/bin/bash 

g++ -O0 -c -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -I/home/ahrrass/cplex/CPLEX_Studio128/cplex/include -I/home/ahrrass/cplex/CPLEX_Studio128/concert/include  cutstock.cpp -o cutstock.o 

g++ -O0 -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -I/home/ahrrass/cplex/CPLEX_Studio128/cplex/include -I/home/ahrrass/cplex/CPLEX_Studio128/concert/include  -L/home/ahrrass/cplex/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic -L/home/ahrrass/cplex/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic -o cutstock cutstock.o -lconcert -lilocplex -lcplex -lm -lpthread -ldl