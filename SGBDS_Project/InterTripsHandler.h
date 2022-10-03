#pragma once
#include <set>
#include "InterTrip.h"
#include <vector>
#include <string>

using namespace std;

set<InterTrip*> handle_file_stream_inter_trips(ifstream& i_file, set<BusStation*> busStationsSet);
InterTrip* buildInterTrip(vector<string> stringData, set<BusStation*> busStationsSet);


