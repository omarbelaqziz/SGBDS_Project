#!/bin/bash 

export C_A="0.2"
export C_V="0.4"
export FIX_COST="1000.0"
export MIN_WAIT="45"

DEPOT_FILE=../data/test50.txt


./run.sh $DEPOT_FILE ../output/clusters/asdfa.txt ../output/stats/stats.txt