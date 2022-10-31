#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include <unordered_map>
#include <map>  
#include <ctime>
#include "../SGBDS_Project/preproc.h"
using namespace std; 

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;


void handle_file_stream_bus_trips(unordered_map<string, multiset<BusTrip> *> &tripsMap, ifstream &i_file, set<BusStation> *busStationsSet);
void buildBusTrip(BusTrip& busTrip, vector<string> rawData, set<BusStation>& busStationsSet);
static time_t dateTimeStringToTimeObject(string date, string time);
BusStation* findBusStationById(const set<BusStation*>& busStationsSet, string busStationId);
void detach_lines(multiset<BusTrip> &resultTrips, TRIPS_MAP linesTrips);
bool findTripById(BusTrip& bt, string tripId, multiset<BusTrip> busTrips);
