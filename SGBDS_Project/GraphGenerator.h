#pragma once 
#include "preproc.h"

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips,
    ofstream& output_file);

vector<vector<string>> clusters_generator_fromTripsSet(
    INTER_TRIPS stationsTargets,
    set<BusStation>* busStations,
    multiset<BusTrip> trips,
    int mode);
void write_cluster_to_file(ofstream& output_file, vector<vector<string>> allClusters);
void stats_calculator(
    vector<vector<double>>& clustersStats,
    vector<double>& depotStats,
    vector<vector<string>> clusters,
    INTER_TRIPS stationsTargets,
    set<BusStation>* busStations,
    multiset<BusTrip> trips);

void writeStatsIntoScreen(
    const vector<vector<double>>& clustersStats,
    const vector<double>& depotStats
);