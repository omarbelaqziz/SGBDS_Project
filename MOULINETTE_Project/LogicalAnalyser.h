#pragma once
#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include "../SGBDS_Project/preproc.h"

using namespace std;

class LogicalAnalyser
{
private:
    ifstream param_in;
    ifstream solution_in;
    ifstream datafile_in;

    set<BusStation> *busStations;
    INTER_TRIPS interTrips;
    multiset<BusTrip> tripsPopulation;
    TRIPS_MAP tripsStations;

protected:
    LogicalAnalyser(const string param_file,
                    const string solution_file,
                    const string datafile_file);

    static LogicalAnalyser *logicalAnalyser_;

public:
    static LogicalAnalyser *getInstance(const string param_file,
                                        const string solution_file,
                                        const string datafile_file);
    LogicalAnalyser(LogicalAnalyser &other) = delete;
    void operator=(const LogicalAnalyser &) = delete;

    // Business Traitement
    void constructTripsPopulation();
    void showBusTripsPopulation();
    void rulesVerfication(const vector<vector<string>> &clusters, vector<vector<string>> &output_data) const;
    const bool findInterTripByTripId(BusTrip &temp, string tripId) const;
    bool static verifyTwoTrips(string T_1, string T_2);
};
