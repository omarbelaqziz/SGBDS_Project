#!/bin/bash 

if [[ $# -eq 0 ]] ; then 
    echo "Please specify the correct livrable to compile"
    exit 1
fi

# CCOPT="-m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD"
# CCFLAGS=$CCOPT

PREFIX_PATH_STR_OPS="../../SGBDS_Project/StringOperations"
PREFIX_PATH_BUSINESS_ENTITIES="../../SGBDS_Project/BUSINESS_ENTITIES"
PREFIX_PATH_HANDLERS="../../SGBDS_Project/HANDLERS"
PREFIX_PATH_GRAPH_GEN="../../SGBDS_Project/GRAPH_Generators"

LIVRABLE_INDEX=$1

# here we will conidere three livrable then we will have other bash scripts for running the main  
# Livrable 1
if [[ "$1" == "livrable1" ]]; then 
    g++ -g\
        ./main_livrableUn.cpp \
        $PREFIX_PATH_STR_OPS/*.cpp \
        $PREFIX_PATH_BUSINESS_ENTITIES/*.cpp \
        $PREFIX_PATH_HANDLERS/*.cpp \
        $PREFIX_PATH_GRAPH_GEN/*.cpp \
        -o livrable1

# Livrable 2
elif [[ "$1" == "livrable2" ]]; then 
    g++ -g\
        ./main_livrableDeux.cpp \
        $PREFIX_PATH_STR_OPS/*.cpp \
        $PREFIX_PATH_BUSINESS_ENTITIES/*.cpp \
        $PREFIX_PATH_HANDLERS/*.cpp \
        $PREFIX_PATH_GRAPH_GEN/*.cpp \
        -o livrable2
# Livrable 3
elif [[ "$1" == "livrable3" ]]; then
    echo "not cofigured yet"
    
else 
    echo "the name of variable needs to be livrable1 OR livrable2 OR livrable3"
fi