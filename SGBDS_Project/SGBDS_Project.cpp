// SGBDS_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "preproc.h"
#include "GraphGenerator.h"

using namespace std;


BusStation createBusStationFromLine(string line)
{
    string fineLine(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
    vector<string> strings = StringsOperations::split(fineLine);
    return BusStation(strings[0], StringsOperations::stringToBool(strings[1]));
}

int main()
{
    // string filename = "./data/test.txt";
    string filename = FILENAME;
   


    set<BusStation> *busStations = nullptr;
    vector<BusStation> busStationsVector;

    INTER_TRIPS interTrips;
    TRIPS_MAP tripsStations;

    ifstream dataFile;
    dataFile.open(filename, ios::in);
    if (dataFile.is_open())
    {
        string line;
        int lineNumber = 0;
        while (getline(dataFile, line))
        {
            lineNumber++;
            if (line.find("{") != string::npos)
            {
                if (line.find("BusStations") != string::npos)
                {
                    busStations = handle_file_stream_bus_stations(dataFile);
                }
                else if (line.find("BusTrip") != string::npos)
                {
                    handle_file_stream_bus_trips(tripsStations, dataFile, busStations);
                }
                else if (line.find("InterTrips") != string::npos)
                {

                    cout << "Inter Trips op. begins at " << lineNumber << endl;
                    handle_file_stream_inter_trips(interTrips, dataFile, busStations);
                }
            }
            else if (line.find("}") != string::npos)
            {
                cout << "Op. ends at " << line << endl;
            }
        }

        graph_generator(interTrips, busStations, tripsStations);
    }
    else
    {
        cout << "cannot open " << filename << endl;
    }
    dataFile.close();
}
