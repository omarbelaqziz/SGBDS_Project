#pragma once
#include <vector>
#include <set>
#include "../SGBDS_Project/preproc.h"
#include "LogicalAnalyser.h"

/**
 * @brief
 * This class defines the needed methods to calculate different statistics
 *
 */

class StatisticsAnalyser
{

private:
    vector<vector<string>> matrixData;
    ofstream stats_out;

    // depot statistics
    int totalTripDuration;

    int totalTrips;

    int clustersCount;
    int clustersWithOneTrip;
    float coutTotal;

    int HLPCounter;
    int HLPDuration; // + the porcentage of hlps on the total

    int attenteCounter;
    int attenteDuration; // + the porcentage of attentes on the total

public:
    StatisticsAnalyser(vector<vector<string>> &matData, string stats_out);

    vector<float> calculateClusterData(int clusterPosition, vector<string> cluster);
    void handleDepot();
    //
    float coutTotalCalculator();

    // this method will show the needed statistics by each depot
    // in goal to have comparaison between livrables
    void showSubData();
};