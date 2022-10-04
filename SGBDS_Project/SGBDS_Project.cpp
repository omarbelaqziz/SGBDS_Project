// SGBDS_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include "BusStation.h"
#include "BusTrip.h"
#include "StringsOperations.h"
#include "BusStationsHandler.h"
#include "BusTripsHandler.h"
#include "BusInterTripsHandler.h"

#include "TargetInterTrip.h"

using namespace std;

typedef unordered_map<string, multiset<BusTrip> *> TRIPS_MAP;
typedef map<string, set<TargetInterTrip>*> INTER_TRIPS;

BusStation createBusStationFromLine(string line)
{
    string fineLine(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
    vector<string> strings = StringsOperations::split(fineLine);
    return BusStation(strings[0], StringsOperations::stringToBool(strings[1]));
}

int main()
{

    // string filename = "../data/InputDataDepot50_ExistedDeadheadsWithBusLines.txt";
    string filename = "./data/test.txt";
    set<BusStation> *busStations;
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
                    cout << "Bus Stations op. begins at " << lineNumber << endl;
                    busStations = handle_file_stream_bus_stations(dataFile);
                }
                else if (line.find("BusTrip") != string::npos)
                {
                    cout << "Bus OP starts at " << lineNumber << endl;
                    handle_file_stream_bus_trips(tripsStations, dataFile, busStations);

                    cout << "size L_16: " << tripsStations.at("L_16")->size() << endl;
                    for (auto sf : *tripsStations["L_16"])
                    {
                        sf.showBusTrip();
                    }
                    cout << "=====================" << endl; 
                }
                else if (line.find("InterTrips") != string::npos)
                {
                    
                    cout << "Inter Trips op. begins at " << lineNumber << endl;
                    handle_file_stream_inter_trips(interTrips, dataFile, busStations);
                
                    cout << "---------------" << endl; 
                    set<TargetInterTrip>*dd = interTrips["Depot50"]; 
                    set<TargetInterTrip>::iterator it; 
                    for(it = dd->begin(); it!=dd->end(); ++it)
                    {
                        cout << "Depot50" << "===>"; 
                        (*it).showTarget();  
                         
                    }

                    cout << "=====Assuring data ====" << endl;
                    cout << "Depot50 destinations: " << dd->size() <<endl;
                    cout << "stations count: " << busStations->size() << endl;   
                }
            }
            else if (line.find("}") != string::npos)
            {
                cout << "Op. ends at " << line << endl;
            }
        }
    }

    else
    {
        cout << "cannot open " << filename << endl;
    }

    cout << "===============" << endl; 
    

    dataFile.close();
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
