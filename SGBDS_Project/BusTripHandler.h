#include <iostream>
#include <algorithm>
#include <set> 
#include <vector>
#include "BusTrip.hpp"

using namespace std; 

vector<pair<string, string>> handle_file_stream_bus_stations(ifstream& i_file); 
bool isDepot_handler(string isDepot_string);
set<BusStation *> handle_bus_stations(vector<pair<string, string>> stations); 
