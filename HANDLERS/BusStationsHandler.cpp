#include "BusStationsHandler.h"

set<BusStation> *handle_file_stream_bus_stations(ifstream &i_file)
{
    string line;
    vector<string> temp;
    set<BusStation> *bus_stations = new set<BusStation>();

    while (getline(i_file, line))
    {
        if (line.find("}") != string::npos)
        {
            // cout << "Bus Stations op. ends at " << line << endl;
            break;
        }
        temp = StringOps::split(StringOps::ltrim(line));

        bus_stations->insert(BusStation(temp[0], isDepot_handler(temp[1])));
    }

    return bus_stations;
}

bool isDepot_handler(string isDepot_string)
{
    bool result = false;
    char v = isDepot_string.at(0);
    switch (v)
    {
    case '0':
        result = false;
        break;

    case '1':
        result = true;
        break;
    default:
        cerr << RED << "error wile reading the is Depot property --> we'll considere it false for the moment " << RESET << endl;
        break;
    }
    return result;
}

string findDepotId(set<BusStation> *busStations)
{
    set<BusStation>::iterator itr;
    string depotId = "";
    for (itr = busStations->begin();
         itr != busStations->end(); itr++)
    {
        if (itr->getIsDepot() == true)
            return itr->getId();
    }

    cerr << RED << "No Depot found in the data set please add one " << RESET << endl; 
    exit(EXIT_FAILURE); 
    return depotId;
}
