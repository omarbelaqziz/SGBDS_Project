#include "InterTripsHandler.h"
#include "StringsOperations.h"

using namespace std;
set<InterTrip*> handle_file_stream_inter_trips(ifstream& i_file, set<BusStation*> busStationsSet)
{
    string line;
    vector<string> stringData;
    set<InterTrip*> inter_trips;

    while (getline(i_file, line)) {
        if (line.find("}") != string::npos) {
            cout << "Inter Trips op. ends at " << line << endl;
            break;
        }
        stringData = StringsOperations::split(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
        InterTrip* intertrip = buildInterTrip(stringData, busStationsSet);
        //test if stations exist
        inter_trips.insert(intertrip);
    }

    return inter_trips;
}
InterTrip* buildInterTrip(vector<string> stringData, set<BusStation*> busStationsSet)
{
    // BusStation* depBusStation = findBusStationById(busStationsSet,rawData[2]);
    auto depBusStation = busStationsSet.find(new BusStation(stringData[2], false));

    auto arrivalBusStation = busStationsSet.find(new BusStation(stringData[3], false));
    int duration = stoi(stringData[4]);

    return new InterTrip(
        stringData[1],
        *depBusStation,
        *arrivalBusStation,
        duration
    );
}