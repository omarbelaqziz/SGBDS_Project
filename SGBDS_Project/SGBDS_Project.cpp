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
#include ""
using namespace std;

BusStation createBusStationFromLine(string line) {
    string fineLine(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
    vector<string> strings = StringsOperations::split(fineLine);
    return BusStation(strings[0], StringsOperations::stringToBool(strings[1]));
}



int main()
{

    //string filename = "../data/InputDataDepot50_ExistedDeadheadsWithBusLines.txt";
    string filename = "./data/test.txt";
    set<BusStation> busStationsSet;
    set<BusStation*> busStations;
    vector<BusStation> busStationsVector;
    ifstream dataFile;
    dataFile.open(filename, ios::in);
    if (dataFile.is_open()) {
        string line;
        int lineNumber = 0;
        while (getline(dataFile, line)) {
            lineNumber++;
            if (line.find("{") != string::npos) {
                if (line.find("BusStations") != string::npos) {
                    cout << "Bus Stations op. begins at " << lineNumber << endl;
                    /*while (getline(dataFile, line)) {
                        if (line.find("}") != string::npos) {
                            cout << "Bus Stations op. ends at " << line << endl;
                            break;
                        }
                        busStationsVector.push_back(createBusStationFromLine(line));

                    }*/
                    vector<pair<string, string>> tokens = handle_file_stream_bus_stations(dataFile);
                    set<BusStation*> busStations = handle_bus_stations(tokens);
                
                    for(auto v : busStations) 
                    {
                        v->showBusStation();  
                    }
                }
                else if (line.find("BusTrip") != string::npos) {
                    cout << "Bus OP starts at " << lineNumber << endl;
                    unordered_map<string, set<BusTrip>> map = handle_file_stream_bus_trips(dataFile, busStations);
                }
                else if (line.find("InterTrips") != string::npos) {
                    cout << "Inter Trips op. begins at " << lineNumber << endl;
                    interTrips = handle_file_stream_inter_trips(dataFile, busStations);

                    for (auto v : interTrips)
                    {
                        v->showInterTrips();
                    }
                }
            }
            else if (line.find("}") != string::npos) {
                cout << "Op. ends at " << line << endl;
            }
        }
    }
    else {
        cout << "cannot open " << filename << endl;
    }
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
