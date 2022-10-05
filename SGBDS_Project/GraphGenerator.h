#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <unordered_map>
#include <map>
#include <ctime>
#include "BusStation.h"
#include "TargetInterTrip.h"
#include "BusTrip.h"

using namespace std;

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;
typedef map<string, set<TargetInterTrip> *> INTER_TRIPS;
typedef vector<pair<BusTrip, bool>> TEMP_TYPE;

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips);