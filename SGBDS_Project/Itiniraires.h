#pragma once

#include <map>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include "BusTrip.h"

using namespace std;


map<string, vector<vector<BusTrip>>> extractItineraries(unordered_map<string, multiset<BusTrip>*> trips);

