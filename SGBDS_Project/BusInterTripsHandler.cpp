#include "BusInterTripsHandler.h"
#include "StringsOperations.h"

using namespace std;
void handle_file_stream_inter_trips(set<InterTrip> &inter_trips, ifstream &i_file, set<BusStation> *busStationsSet)
{
    string line;
    vector<string> stringData;

    while (getline(i_file, line))
    {
        if (line.find("}") != string::npos)
        {
            cout << "Inter Trips op. ends at " << line << endl;
            break;
        }
        stringData = StringsOperations::split(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
        InterTrip intertrip;
        buildInterTrip(intertrip, stringData, *busStationsSet);
        // test if stations exist
        inter_trips.insert(intertrip);
    }
}
void buildInterTrip(InterTrip &inter_tripp, vector<string> stringData, set<BusStation> &busStationsSet)
{
    // BusStation* depBusStation = findBusStationById(busStationsSet,rawData[2]);
    auto depBusStation = busStationsSet.find(BusStation(stringData[2], false));

    auto arrivalBusStation = busStationsSet.find(BusStation(stringData[3], false));
    int duration = stoi(stringData[4]);

    inter_tripp.id = stringData[1];
    inter_tripp.departureBusStation = &(*depBusStation);
    inter_tripp.arrivalBusStation = &(*arrivalBusStation);
    inter_tripp.duration = duration;
}