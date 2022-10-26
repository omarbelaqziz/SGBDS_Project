#include <iostream>
#include "LexicalAnalyser.h"
#include "LogicalAnalyser.h"
#include "StatisticsAnalyser.h"

/**
 * Ex:
 *
 * Cluster1 : Depot T_1254 WS(5) T_2413 HLP(5, 6) T_2412 Depot;
 *
 * Moulinette : Is a component that will double check the availibility of the data
 * it needs to verify the folowing :
 *  - the chronological order of the trips
 *  - The attente not > to 45
 *  - each Trip needs to be covered one at a time
 *  - station Arr = station Dep
 *  - if not HLP needs to be <= connection between tripF & tripS
 */

#define PATTERN "cluster [0-9][0-9]* : (T_[0-9][0-9]*( HLP | WS ))*T_[0-9][0-9]* ;"
#define SOL_FILENAME "../output/out60oc.txt"

#define DATA_FILE "../data/test60.txt"
#define PARAM_FILE "../config/param.in"

int main(int argc, char const *argv[])
{

    // lexical analysis
    vector<vector<string>> clusters;
    LexicalAnalyser *lexicalAnalyser = LexicalAnalyser::getInstance(SOL_FILENAME, PATTERN);
    lexicalAnalyser->fileMatchLexicalReqs(clusters);

    vector<vector<string>> output_data;
    // logical analysis
    LogicalAnalyser *logicalAnalyser = LogicalAnalyser::getInstance(PARAM_FILE, SOL_FILENAME, DATA_FILE);
    // logicalAnalyser->showBusTripsPopulation();

    logicalAnalyser->rulesVerfication(clusters, output_data);

    string stats_out = "../output/stats.txt"; 
    StatisticsAnalyser *statisticsAnalyser = new StatisticsAnalyser(output_data, stats_out); 
    statisticsAnalyser->handleDepot(); 

    // calcule du cout total

    return 0;
}
