#include "BusStationsHandler.h"
#include "StringsOperations.h"

set<BusStation*> handle_file_stream_bus_stations(ifstream& i_file) {
    string line;
    vector<string> temp;
    set<BusStation*> bus_stations;

    while (getline(i_file, line))
    {
        if (line.find("}") != string::npos) {
            cout << "Bus Stations op. ends at " << line << endl;
            break;
        }
        temp = StringsOperations::split(StringsOperations::ltrim(line));
        

        bus_stations.insert(new BusStation(temp[0], isDepot_handler(temp[1])));
    }

    return bus_stations;
}

bool isDepot_handler(string isDepot_string)
{
    bool result = false;
    if (isDepot_string.size() == 2)
    {
        char v = isDepot_string.at(0);
        (v == '0') ? result = false : result = true;
    }
    // handle the error 
    return result;
}

