#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include "BusStation.h"
#include "InterTrip.h"

using namespace std; 

void handle_file_stream_inter_trips(set<InterTrip>& inter_trips, ifstream& i_file, set<BusStation>* busStationsSet);
void buildInterTrip(InterTrip& inter_tripp, vector<string> stringData, set<BusStation>& busStationsSet);

