#include "BusTripsHandler.h"
#include "StringsOperations.h"

vector<vector<string>> handle_file_stream_bus_trips(ifstream &i_file)
{
    string line; 
    vector<string> temp; 
    vector<vector<string>> tokens; 

    while(getline(i_file, line)) {
        temp = StringsOperations::split(StringsOperations::ltrim(line)); 
        tokens.push_back(temp); 
    }

    return tokens; 
}

unordered_map<string, set<BusTrip *>> handle_bus_trips(vector<vector<string>> trips)
{
    
}