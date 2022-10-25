#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include <unordered_map>
#include <map>  
#include <ctime>
#include "BusTrip.h"

using namespace std; 


void handle_file_stream_bus_trips(unordered_map<string, multiset<BusTrip> *> &tripsMap, ifstream &i_file, set<BusStation> *busStationsSet);
void buildBusTrip(BusTrip& busTrip, vector<string> rawData, set<BusStation>& busStationsSet);
static time_t dateTimeStringToTimeObject(string date, string time);
BusStation* findBusStationById(const set<BusStation*>& busStationsSet, string busStationId);
bool findTripById(BusTrip& bt, string tripId, multiset<BusTrip> busTrips);
//unordered_map<string, set<BusTrip*>> handle_bus_trips(vector<vector<string>> trips);
