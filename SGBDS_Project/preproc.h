
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <ctime>
#include <memory>
#include <algorithm>

#include "BusStation.h"
#include "BusTrip.h"
#include "StringsOperations.h"
#include "BusStationsHandler.h"
#include "BusTripsHandler.h"
#include "BusInterTripsHandler.h"

#include "TargetInterTrip.h"
#include "StatisticsCalculator.h"

// #include "GraphGenerator.h"

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;
typedef map<string, set<TargetInterTrip> *> INTER_TRIPS;
typedef vector<pair<BusTrip, bool>> TEMP_TYPE;

#define FILENAME "./data/test.txt"



#define c_a 0.4
#define c_v 0.6

using namespace std; 