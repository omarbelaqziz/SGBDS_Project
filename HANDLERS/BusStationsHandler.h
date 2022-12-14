#pragma once 
#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include "../SGBDS_Project/preproc.h"

using namespace std; 

set<BusStation> * handle_file_stream_bus_stations(ifstream& i_file);
bool isDepot_handler(string isDepot_string);
string findDepotId(set<BusStation>* busStations);