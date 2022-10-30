#include "BusInterTripsHandler.h"

using namespace std;

void handle_file_stream_inter_trips(map<string, set<TargetInterTrip> *> &inter_trips, ifstream &i_file, set<BusStation> *busStationsSet)
{

    string line;
    vector<string> stringData;

    while (getline(i_file, line))
    {
        if (line.find("}") != string::npos)
        {
            cout << endl;
            break;
        }
        stringData = StringOps::split(StringOps::removeLastChar(StringOps::ltrim(StringOps::rtrim(line))));
        

        auto depBusStation = busStationsSet->find(BusStation(stringData[1], false));

        auto arrivalBusStation = busStationsSet->find(BusStation(stringData[2], false));

    

        int duration = stoi(stringData[3]);
        string interTripId = stringData[0];
        // struct Temp te;
        if (depBusStation == busStationsSet->end() && arrivalBusStation == busStationsSet->end())
        {
            cout << "No station found" << endl;
            exit(-1); 
        }

        set<TargetInterTrip> *DepTargets;
        TargetInterTrip targetInterTrip(&(*arrivalBusStation), interTripId, duration); 
        

        if (inter_trips.count((*depBusStation).id) > 0)
        {
            
            DepTargets = inter_trips[(*depBusStation).getId()];
            DepTargets->insert(targetInterTrip);
        }
        else
        {
            DepTargets = new set<TargetInterTrip>();
            DepTargets->insert(targetInterTrip);
            inter_trips[(*depBusStation).getId()] = DepTargets;
        }
    }
}