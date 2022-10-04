// SGBDS_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <ctime>
#include "BusStation.h"
#include "BusTrip.h"
#include "StringsOperations.h"
#include "BusStationsHandler.h"
#include "BusTripsHandler.h"
#include "BusInterTripsHandler.h"
#include "InterTrip.h"
#include "StatisticsCalculator.h"

using namespace std;

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;
typedef set<InterTrip> INTER_TRIPS;

BusStation createBusStationFromLine(string line)
{
    string fineLine(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
    vector<string> strings = StringsOperations::split(fineLine);
    return BusStation(strings[0], StringsOperations::stringToBool(strings[1]));
}

int main()
{


    // string filename = "../data/InputDataDepot50_ExistedDeadheadsWithBusLines.txt";
    string filename = "../data/test.txt";
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
                /*else if (line.find("InterTrips") != string::npos)
                {
                    handle_file_stream_inter_trips(interTrips, dataFile, busStations)
                }*/
            }
        }
    }
    else
    {
        cout << "cannot open " << filename << endl;
    } 
    dataFile.close();

    // showing statistics
    showAllStatistics(tripsStations);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
