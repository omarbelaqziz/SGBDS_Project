#pragma once

#include "../SGBDS_Project/preproc.h"

void detach_lines(multiset<BusTrip> &resultTrips, TRIPS_MAP linesTrips);
void heuristic_graph_builder(multiset<BusTrip> busTripsPopulation, ofstream &output_file, set<BusStation> *busStations, INTER_TRIPS stationsTargets); 