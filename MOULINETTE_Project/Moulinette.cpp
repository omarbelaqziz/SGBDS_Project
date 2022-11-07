#include <iostream>
#include <chrono>
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

// #define PATTERN "cluster [0-9][0-9]* : (T_[0-9][0-9]*( HLP | WS ))*T_[0-9][0-9]* ;"
#define PARAM_FILE "../config/param.in"

int main(int argc, char const *argv[])
{
    auto start = chrono::high_resolution_clock::now();

    // cmd form to tun this program : ./moulinette dataFile.txt onlyCluster.txt stats_out.txt 

    // lexical analysis
    vector<vector<string>> clusters;
    LexicalAnalyser *lexicalAnalyser = LexicalAnalyser::getInstance(argv[2], PATTERN);
    lexicalAnalyser->fileMatchLexicalReqs(clusters);

    cout << "Lexical analyser :" <<  GREEN << "DONE" << RESET << endl; 

    vector<vector<string>> output_data;
    // logical analysis
    LogicalAnalyser *logicalAnalyser = LogicalAnalyser::getInstance(PARAM_FILE, argv[2], argv[1]);

    logicalAnalyser->rulesVerfication(clusters, output_data);

    cout << "Logical analyser :" <<  GREEN << "DONE" << RESET << endl << endl;

    StatisticsAnalyser *statisticsAnalyser = new StatisticsAnalyser(output_data, argv[3]);
    statisticsAnalyser->handleDepot();

    cout << "Statistics analyser :" <<  GREEN << "DONE" << RESET << endl << endl;

    statisticsAnalyser->showSubData(); 


    // free up the instances to de-allocate memory 
    free(lexicalAnalyser);
    free(logicalAnalyser);
    delete statisticsAnalyser;

    lexicalAnalyser = nullptr;
    logicalAnalyser = nullptr;

    auto end = chrono::high_resolution_clock::now();
	auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << endl << "Elapsed Time(s): " << Elapsed.count() / 1000.0 << endl;

    return 0;
}
