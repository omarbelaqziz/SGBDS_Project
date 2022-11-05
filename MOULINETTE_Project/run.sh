#!/bin/bash 

export C_A="0.2"
export C_V="0.4"
export FIX_COST="1000.0"
export MIN_WAIT="45"
export PATTERN_CLUSTERS="cluster [0-9][0-9]* : (T_[0-9][0-9]*( HLP | WS ))*T_[0-9][0-9]* ;"

# $2 and $3 will hold the datafile and destination file and $4 only cluster file 

if [[ $# -ne 3 ]] ; then 
    echo "Please specify all the needed arguments to run the Moulinette correctly"
    exit 0
fi

./moulinette $1 $2 $3