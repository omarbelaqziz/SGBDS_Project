#include "BusStationsHandler.h"
#include "StringsOperations.h"

vector<pair<string, string>> handle_file_stream_bus_stations(ifstream& i_file){
    string line; 
    vector<string> temp; 
    vector<pair<string, string>> tokens; 

    while (getline(i_file, line))
    {
        temp = StringsOperations::split(StringsOperations::ltrim(line)); 
        pair<string, string> p(temp[0], temp[1]); 
        tokens.push_back(p); 
    }
    
    return tokens; 
}


bool isDepot_handler(string isDepot_string)
{
    bool result = false; 
    if(isDepot_string.size() == 2)
    {
        char v = isDepot_string.at(0); 
        (v == '0') ? result = false: result = true; 
    }
    // handle the error 
    return result; 
}


set<BusStation *> handle_bus_stations(vector<pair<string, string>> stations) {
    set<BusStation *> bus_stations; 
    for(auto station: stations){
        bus_stations.insert(new BusStation(station.first, isDepot_handler(station.second))); 
    }

    return bus_stations; 
}

