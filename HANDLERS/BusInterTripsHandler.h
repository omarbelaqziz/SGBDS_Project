#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include<map>
#include "../SGBDS_Project/preproc.h"

using namespace std; 



void handle_file_stream_inter_trips(map<string, set<TargetInterTrip>*> &inter_trips, ifstream &i_file, set<BusStation> *busStationsSet);

