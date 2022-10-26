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
    int totalTripDuration = 0;

    int totalTrips = 0; 

    int clustersCount = 0;
    int clustersWithOneTrip = 0;
    int coutTotal = 0;

    int HLPCounter = 0;
    int HLPDuration = 0; // + the porcentage of hlps on the total

    int attenteCounter = 0;
    int attenteDuration = 0; // + the porcentage of attentes on the total

public:
    StatisticsAnalyser(vector<vector<string>> &matData, string stats_out)
    {
        this->matrixData = matData;
        this->stats_out.open(stats_out);
        if (!this->stats_out.is_open())
        {
            cerr << "Sorry we can't open the file : " << stats_out << endl;
            exit(-1);
        }
    }

    vector<int> calculateClusterData(int clusterPosition, vector<string> cluster)
    {
        int totalClusterDuration = 0;

        // when == 1 add it to clusters with one trip
        int totalTripsCluster = 0;
        int coutTotalCluster = 0;

        int HLPClusterCounter = 0;
        int HLPClusterDuration = 0; // + the porcentage of hlps on the total

        int attenteClusterCounter = 0;
        int attenteClusterDuration = 0; // + the porcentage of attentes on the total

        for (auto str : cluster)
        {
            vector<string> temp = StringOps::split(StringOps::ltrim(str));
            switch (temp.size())
            {
            // attente
            case 1:
                totalClusterDuration += stoi(temp[0]);
                attenteClusterDuration += stoi(temp[0]);
                attenteClusterCounter++;
                coutTotalCluster += (stoi(temp[0]) * c_a);
                break;

            // hlp or from or to depot
            case 2:
                totalClusterDuration += stoi(temp[0]);
                if (stoi(temp[0]) != 0)
                {
                    totalClusterDuration += (stoi(temp[0]) + stoi(temp[1]));
                    HLPClusterCounter++;
                    HLPClusterDuration += (stoi(temp[0]) + stoi(temp[1]));
                    coutTotalCluster += (stoi(temp[0]) * c_v + stoi(temp[1]) * c_a);
                }
                break;

            // trip
            case 3:
                totalTripsCluster++;
                totalClusterDuration += stoi(temp[2]);

                break;
            default:
                break;
            }
        }

        int hlpPour = ((HLPClusterDuration * 1.0) / (totalClusterDuration * 1.0)) * 100;
        int attPour = ((attenteClusterDuration * 1.0) / (totalClusterDuration * 1.0)) * 100;

        vector<int> clusterStats = {clusterPosition, totalClusterDuration, coutTotalCluster, HLPClusterCounter, HLPClusterDuration, hlpPour, attenteClusterDuration, attPour, totalTripsCluster};

        this->totalTripDuration += totalClusterDuration;
        this->clustersCount++;
        if (totalTripsCluster == 1)
            this->clustersWithOneTrip++;
        this->coutTotal += coutTotalCluster;
        this->HLPCounter += HLPClusterCounter;
        this->HLPDuration += HLPClusterDuration; // + the porcentage of hlps on the total
        this->attenteCounter += attenteClusterCounter;
        this->attenteDuration += attenteClusterDuration;

        this->totalTrips += totalTripsCluster; 

        return clusterStats;
    }

    void handleDepot()
    {
        int i = 1;
        cout << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
        cout << "| Cluster | Duration | Cluster's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips |" << endl;
        cout << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;

        for (auto cluster : this->matrixData)
        {
            vector<int> data = this->calculateClusterData(i, cluster);
            cout << "|" << setw(9) << i;
            cout << "|" << setw(10) << data[0];
            cout << "|" << setw(16) << data[1];
            cout << "|" << setw(16) << data[2];
            cout << "|" << setw(14) << data[3];
            cout << "|" << setw(17) << data[4];
            cout << "|" << setw(15) << data[5];
            cout << "|" << setw(18) << data[6];
            cout << "|" << setw(17) << data[7] << "|" << endl;
            i++;
        }

        cout << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
        cout << endl;

        cout << "DEPOT STATS : " << endl;
        cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
        cout << "| Total Duration | Depot's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips | Number of Clusters |" << endl;
        cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
        cout << "|" << setw(16) << this->totalTripDuration;
        cout << "|" << setw(14) << this->clustersCount;
        cout << "|" << setw(16) << this->HLPCounter;
        cout << "|" << setw(14) << this->HLPDuration;
        cout << "|" << setw(17) << (this->HLPDuration / this->totalTripDuration) * 100;
        cout << "|" << setw(15) << this->attenteDuration;
        cout << "|" << setw(18) << (this->attenteDuration / this->totalTripDuration) * 100;
        cout << "|" << setw(17) << this->totalTrips;
        cout << "|" << setw(20) << this->clustersCount << "|" << endl;
        cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    }

    //
    int coutTotalCalculator()
    {
        return this->coutTotal;
    }
};