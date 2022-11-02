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
#define SOL_FILENAME "../output/out50oc.txt"

#define DATA_FILE "../data/test50.txt"
#define PARAM_FILE "../config/param.in"

int main(int argc, char const *argv[])
{
    char filenames[][19] = {
        "../data/test50.txt",
        "../data/test54.txt",
        "../data/test55.txt",
        "../data/test56.txt",
        "../data/test57.txt",
        "../data/test58.txt",
        "../data/test59.txt",
        "../data/test60.txt"};

    char statsout[][20] = {
        "../data/stats50.txt",
        "../data/stats54.txt",
        "../data/stats55.txt",
        "../data/stats56.txt",
        "../data/stats57.txt",
        "../data/stats58.txt",
        "../data/stats59.txt",
        "../data/stats60.txt"};

    char output_files[][20] = {
        "../output/out50.txt",
        "../output/out54.txt",
        "../output/out55.txt",
        "../output/out56.txt",
        "../output/out57.txt",
        "../output/out58.txt",
        "../output/out59.txt",
        "../output/out60.txt"};

    char output_files_onlycluster[][22] = {
        "../output/out50oc.txt",
        "../output/out54oc.txt",
        "../output/out55oc.txt",
        "../output/out56oc.txt",
        "../output/out57oc.txt",
        "../output/out58oc.txt",
        "../output/out59oc.txt",
        "../output/out60oc.txt"};


// cmd form ./ll test50.txt out50oc.txt
    if (argc == 3)
    {
        // lexical analysis
            vector<vector<string>> clusters;
            LexicalAnalyser *lexicalAnalyser = LexicalAnalyser::getInstance(argv[2], PATTERN);
            lexicalAnalyser->fileMatchLexicalReqs(clusters);

            vector<vector<string>> output_data;
            // logical analysis
            LogicalAnalyser *logicalAnalyser = LogicalAnalyser::getInstance(PARAM_FILE, argv[2], argv[1]);
            
            logicalAnalyser->rulesVerfication(clusters, output_data);

            StatisticsAnalyser *statisticsAnalyser = new StatisticsAnalyser(output_data, statsout[0]); 
            statisticsAnalyser->handleDepot(); 


            //  free(lexicalAnalyser);
            //  free(logicalAnalyser);
            //delete statisticsAnalyser;

            // lexicalAnalyser = nullptr;
            // logicalAnalyser = nullptr;

            // clusters.erase(clusters.begin(), clusters.end()); 
            // output_data.erase(output_data.begin(), output_data.end()); 

            // statisticsAnalyser = nullptr;
    }
    else {
        cout << "please run the following form of the commande : ./moulinette DATA_FILE SOLUTION_FILE" << endl;
        return -1;
    }
        

    return 0;
}
