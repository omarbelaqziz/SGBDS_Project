#pragma once 
#include "preproc.h"

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips,
    ofstream& output_file);