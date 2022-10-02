#include <bits/stdc++.h>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <algorithm>
#include "StringsOperations.h"
#include "BusStation.h"
#include "BusStationsHandler.h"
using namespace std;

int main(int argc, char const *argv[])
{
    string fn("./resources/test_bus_trips.txt");
    ifstream i_file;

    i_file.open(fn);

    if (i_file.is_open())
    {
        vector<pair<string, string>> tokens = handle_file_stream_bus_stations(i_file);
        set<BusStation *> busStations = handle_bus_stations(tokens);

        for (auto s : busStations)
        {
            s->showBusStation();
        }
    }
    else
    {
        cerr << "Could not open the file: " << fn << endl;
        return EXIT_FAILURE;
    }

    i_file.close(); 


    return 0;
}
