#!/bin/bash 

export C_A="0.2"
export C_V="0.4"
export FIX_COST="1000.0"
export MIN_WAIT="45"
export PATTERN_CLUSTERS="cluster [0-9][0-9]* : (T_[0-9][0-9]*( HLP | WS ))*T_[0-9][0-9]* ;"


# $2 and $3 will hold the datafile and destination file and $4 only cluster file 

if [[ $# -ne 4 ]] ; then 
    echo "Please specify the correct livrable to run"
    exit 1
fi

# Livrable 1
if [[ "$1" == "livrable1" ]]; then 
    gdb --args livrable1 $2 $3 $4
    # ./livrable1 $2 $3 $4

# Livrable 2
elif [[ "$1" == "livrable2" ]]; then 
    gdb --args livrable2 $2 $3 $4
    # ./livrable2 $2 $3 $4

# Livrable 3
elif [[ "$1" == "livrable3" ]]; then
    ./livrable3 $2 $3 $4 

else 
    echo "the name of variable needs to be livrable1 OR livrable2 OR livrable3"
fi