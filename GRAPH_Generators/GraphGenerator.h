#pragma once 
#include "../SGBDS_Project/preproc.h"

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips,
    ofstream& output_file);

vector<vector<string>> clusters_generator_fromTripsSet(
    INTER_TRIPS stationsTargets,
    set<BusStation>* busStations,
    multiset<BusTrip> trips);
void write_cluster_to_file(ofstream& output_file, vector<vector<string>> allClusters);
void stats_calculator(
    vector<vector<double>>& clustersStats,
    vector<double>& depotStats,
    vector<vector<string>> clusters,
    INTER_TRIPS stationsTargets,
    set<BusStation>* busStations,
    multiset<BusTrip> trips);

void writeStatsIntoScreen(
    const vector<vector<double>> &clustersStats,
    const vector<double> &depotStats, 
    ofstream &stats_out_stream);
     
bool twoTripsCost(float& cost, 
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    BusTrip firstTrip,
    BusTrip secondTrip,
    string& type);

vector<vector<string>> optimize_generated_solution(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    multiset<BusTrip> trips,
    vector<vector<string>> solution);