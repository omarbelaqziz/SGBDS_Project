#pragma once 

#include "../SGBDS_Project/preproc.h"

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips,
    ofstream& output_file);