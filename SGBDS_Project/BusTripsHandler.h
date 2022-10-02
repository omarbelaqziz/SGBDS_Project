#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include <unordered_map>
#include <map>  
#include <ctime>
#include "BusTrip.h"

using namespace std; 

unordered_map<string, set<BusTrip>> handle_file_stream_bus_trips(ifstream& i_file, set<BusStation*> busStationsSet);
BusTrip* buildBusTrip(vector<string> rawData, set<BusStation*> busStationsSet);
static time_t dateTimeStringToTimeObject(string date, string time);

BusStation* findBusStationById(const set<BusStation*>& busStationsSet, string busStationId);

//unordered_map<string, set<BusTrip*>> handle_bus_trips(vector<vector<string>> trips);
