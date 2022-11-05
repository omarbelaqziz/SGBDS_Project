#!/bin/bash 

# CCOPT="-m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD"
# CCFLAGS=$CCOPT

PREFIX_PATH_STR_OPS="../../SGBDS_Project/StringOperations"
PREFIX_PATH_BUSINESS_ENTITIES="../../SGBDS_Project/BUSINESS_ENTITIES"
PREFIX_PATH_HANDLERS="../../SGBDS_Project/HANDLERS"
PREFIX_PATH_GRAPH_GEN="../../SGBDS_Project/GRAPH_Generators"

LIVRABLE_INDEX=$1

# here we will conidere three livrable then we will have other bash scripts for running the main  

g++ -g\
    ./*.cpp \
    $PREFIX_PATH_STR_OPS/*.cpp \
    $PREFIX_PATH_BUSINESS_ENTITIES/*.cpp \
    $PREFIX_PATH_HANDLERS/*.cpp \
    $PREFIX_PATH_GRAPH_GEN/*.cpp \
    -o moulinette