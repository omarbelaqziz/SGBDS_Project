#include "StatisticsAnalyser.h"

StatisticsAnalyser::StatisticsAnalyser(vector<vector<string>> &matData, string stats_out)
{
    this->matrixData = matData;
    this->stats_out.open(stats_out);
    if (!this->stats_out.is_open())
    {
        cerr << "Sorry we can't open the file : " << stats_out << endl;
        exit(-1);
    }

    this->totalTripDuration = 0;
    this->totalTrips = 0;
    this->clustersCount = 0;
    this->clustersWithOneTrip = 0;
    this->coutTotal = 0.0;
    this->HLPCounter = 0;
    this->HLPDuration = 0;
    this->attenteCounter = 0;
    this->attenteDuration = 0;
}

vector<float> StatisticsAnalyser::calculateClusterData(int clusterPosition, vector<string> cluster)
{
    int totalClusterDuration = 0;

    // when == 1 add it to clusters with one trip
    int totalTripsCluster = 0;
    float coutTotalCluster = 0.0;

    int HLPClusterCounter = 0;
    int HLPClusterDuration = 0; // + the porcentage of hlps on the total

    int attenteClusterCounter = 0;
    int attenteClusterDuration = 0; // + the porcentage of attentes on the total

    for (auto str : cluster)
    {
        vector<string> temp = StringOps::split(StringOps::ltrim(str));
        totalClusterDuration += stoi(temp[0]);
        switch (temp.size())
        {
        // attente
        case 1:
            attenteClusterDuration += stoi(temp[0]);
            attenteClusterCounter++;
            break;

        // hlp or from or to depot
        case 2:

            HLPClusterCounter++;
            HLPClusterDuration += stoi(temp[0]);
            attenteClusterDuration += stoi(temp[1]);
            totalClusterDuration += (stoi(temp[1]));
            break;

        // trip
        case 3:
            totalTripsCluster++;
            break;

        default:
            break;
        }

        coutTotalCluster = (attenteClusterDuration * c_a) + (HLPClusterDuration * c_v);
    }

    float hlpPour = ((HLPClusterDuration * 1.0) / (totalClusterDuration * 1.0)) * 100;
    float attPour = ((attenteClusterDuration * 1.0) / (totalClusterDuration * 1.0)) * 100;

    vector<float> clusterStats = {(float)clusterPosition, (float)(totalClusterDuration * 1.0), coutTotalCluster, (float)(HLPClusterCounter * 1.0), (float)(HLPClusterDuration * 1.0), hlpPour, (float)(attenteClusterDuration * 1.0), attPour, (float)(totalTripsCluster * 1.0)};

    this->totalTripDuration += totalClusterDuration;
    this->clustersCount++;
    if (totalTripsCluster == 1)
        this->clustersWithOneTrip++;
    this->coutTotal += coutTotalCluster;
    this->HLPCounter += (HLPClusterCounter - 2);
    this->HLPDuration += HLPClusterDuration; // + the porcentage of hlps on the total
    this->attenteCounter += attenteClusterCounter;
    this->attenteDuration += attenteClusterDuration;

    this->totalTrips += totalTripsCluster;

    return clusterStats;
}

void StatisticsAnalyser::handleDepot()
{
    int i = 1;
    this->stats_out << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
    this->stats_out << "| Cluster | Duration | Cluster's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips |" << endl;
    this->stats_out << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;

    for (auto cluster : this->matrixData)
    {
        vector<float> data = this->calculateClusterData(i, cluster);
        this->stats_out << "|" << setw(9) << i;
        this->stats_out << "|" << setw(10) << data[1];
        this->stats_out << "|" << setw(16) << data[2];
        this->stats_out << "|" << setw(16) << data[3];
        this->stats_out << "|" << setw(14) << data[4];
        this->stats_out << "|" << setw(17) << data[5];
        this->stats_out << "|" << setw(15) << data[6];
        this->stats_out << "|" << setw(18) << data[7];
        this->stats_out << "|" << setw(17) << data[8] << "|" << endl;
        i++;
    }

    this->stats_out << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
    this->stats_out << endl;

    this->stats_out << "DEPOT STATS : " << endl;
    this->stats_out << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    this->stats_out << "| Total Duration | Depot's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips | Number of Clusters |" << endl;
    this->stats_out << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    this->stats_out << "|" << setw(16) << this->totalTripDuration;
    this->stats_out << "|" << setw(14) << this->coutTotalCalculator();
    this->stats_out << "|" << setw(16) << this->HLPCounter;
    this->stats_out << "|" << setw(14) << this->HLPDuration;
    this->stats_out << "|" << setw(17) << (this->HLPDuration * 1.0 / this->totalTripDuration * 1.0) * 100;
    this->stats_out << "|" << setw(15) << this->attenteDuration;
    this->stats_out << "|" << setw(18) << (this->attenteDuration * 1.0 / this->totalTripDuration * 1.0) * 100;
    this->stats_out << "|" << setw(17) << this->totalTrips;
    this->stats_out << "|" << setw(20) << this->clustersCount << "|" << endl;
    this->stats_out << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
}

//
float StatisticsAnalyser::coutTotalCalculator()
{
    return this->coutTotal + (1000 * clustersCount);
}

// this method will show the needed statistics by each depot
// in goal to have comparaison between livrables
void StatisticsAnalyser::showSubData()
{
    cout << YELLOW << "for More cluster details you can check : "
         << "../output/stats/out_stats"
         << "[DEPOT_ID].txt" << RESET << endl;
    cout << "DEPOT STATS : " << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    cout << "| Total Duration | Depot's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips | Number of Clusters |" << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    cout << "|" << setw(16) << this->totalTripDuration;
    cout << "|" << setw(14) << this->coutTotalCalculator();
    cout << "|" << setw(16) << this->HLPCounter;
    cout << "|" << setw(14) << this->HLPDuration;
    cout << "|" << setw(17) << (this->HLPDuration * 1.0 / this->totalTripDuration * 1.0) * 100;
    cout << "|" << setw(15) << this->attenteDuration;
    cout << "|" << setw(18) << (this->attenteDuration * 1.0 / this->totalTripDuration * 1.0) * 100;
    cout << "|" << setw(17) << this->totalTrips;
    cout << "|" << setw(20) << this->clustersCount << "|" << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;

    stats_out.close();
}