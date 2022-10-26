#pragma once
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <ctime>
#include <memory>
#include <algorithm>

#include "../BUSINESS_ENTITIES/BusStation.h"
#include "../BUSINESS_ENTITIES/BusTrip.h"
#include "../BUSINESS_ENTITIES/TargetInterTrip.h"

#include "../StringOperations/StringOps.h"

#include "../HANDLERS/BusStationsHandler.h"
#include "../HANDLERS/BusTripsHandler.h"
#include "../HANDLERS/BusInterTripsHandler.h"

#include "StatisticsCalculator.h"

// #include "GraphGenerator.h"
using namespace std; 

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;
typedef map<string, set<TargetInterTrip> *> INTER_TRIPS;
typedef vector<pair<BusTrip, bool>> TEMP_TYPE;

#define FILENAME "../data/test.txt"

#define c_a 0.2
#define c_v 0.4

#define MIN_WAIT 45

