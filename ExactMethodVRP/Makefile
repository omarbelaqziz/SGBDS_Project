SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /home/ahrrass/cplex/CPLEX_Studio128/cplex
CONCERTDIR    = /home/ahrrass/cplex/CPLEX_Studio128/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++ -O0
CC  = gcc -O0
JAVAC = javac 

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
COPT  = -m64 -fPIC -fno-strict-aliasing

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread -ldl
CLNFLAGS  = -lcplex -lm -lpthread -ldl


all:
	make all_c
	make all_cpp

execute: all
	make execute_c
	make execute_cpp
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

EXDIR         = $(CPLEXDIR)/examples
EXINC         = $(EXDIR)/include
EXDATA        = $(EXDIR)/data
EXSRCC        = $(EXDIR)/src/c
EXSRCCX       = $(EXDIR)/src/c_x
EXSRCCPP      = $(EXDIR)/src/cpp

CFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 


#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------



CPP_EX = blend cutstock VRPsimplex

INCLUDES_OFF = ../HANDLERS/*.cpp ../BUSINESS_ENTITIES/*.cpp ../StringOperations/*.cpp

all_c: $(C_EX) $(CX_EX)

all_cpp: $(CPP_EX)

execute_cpp: $(CPP_EX)
	 ./blend
	 ./cutstock
	 ./VRPsimplex
	 

# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(C_EX) $(CX_EX) $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#

blend: blend.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o blend blend.o $(CCLNFLAGS)
blend.o: ./blend.cpp
	$(CCC) -c $(CCFLAGS) ./blend.cpp -o blend.o

cutstock: cutstock.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o cutstock cutstock.o $(CCLNFLAGS)
cutstock.o: ./cutstock.cpp
	$(CCC) -c $(CCFLAGS) ./cutstock.cpp -o cutstock.o

VRPsimplex: cutstock.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) $(INCLUDES_OFF) -o VRPsimplex VRPsimplex.o $(CCLNFLAGS)
VRPsimplex.o: ./cutstock.cpp
	$(CCC) -c $(INCLUDES_OFF) $(CCFLAGS) ./VRPsimplex.cpp -o VRPsimplex.o


# Local Variables:
# mode: makefile
# End: