#include "BusTripsHandler.h"
#include "StringsOperations.h"
#include <iterator>

unordered_map<string, set<BusTrip*>> handle_file_stream_bus_trips(ifstream &i_file, set<BusStation*> busStationsSet)
{
    string line; 
    vector<string> rawData; 
    unordered_map<string, set<BusTrip*>> map;
    while(getline(i_file, line)) {
        if (line.find("}") != string::npos) {
            cout << "Bus Trips op. ends at " << line << endl;
            break;
        }
        rawData = StringsOperations::split(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
        BusTrip* busTrip = buildBusTrip(rawData,busStationsSet);
        if (map.count(rawData[0]) > 0) {
            set<BusTrip*>& BSTrips = map[rawData[0]];
            BSTrips.insert(busTrip);
        }
        else {
            set<BusTrip*> BSTrips;
            BSTrips.insert(busTrip);
            map[rawData[0]] = BSTrips;
        }
    }
    return map; 
}

BusTrip* buildBusTrip(vector<string> rawData, set<BusStation*> busStationsSet)
{
    // BusStation* depBusStation = findBusStationById(busStationsSet,rawData[2]);
    auto depBusStation = busStationsSet.find(new BusStation(rawData[2], false));

    auto arrivalBusStation = busStationsSet.find(new BusStation(rawData[5], false));
    time_t depTime = dateTimeStringToTimeObject(rawData[3], rawData[4]);
    time_t arrivalTime = dateTimeStringToTimeObject(rawData[6], rawData[7]);

    return new BusTrip(
        rawData[1],
        *depBusStation,
        *arrivalBusStation,
        depTime,
        arrivalTime
    );
}

time_t dateTimeStringToTimeObject(string date, string time)
{
    tm timeStruct = tm();
    timeStruct.tm_hour = stoi(time.substr(0, 2)) - 1;
    timeStruct.tm_min = stoi(time.substr(3, 2)) - 1;
    timeStruct.tm_year = stoi(date.substr(0, 4)) - 1900;
    timeStruct.tm_mon = stoi(date.substr(5, 2)) - 1;
    timeStruct.tm_mday = stoi(date.substr(8, 2));
    
    return mktime(&timeStruct);
}

BusStation* findBusStationById(const set<BusStation*>& busStationsSet, string busStationId)
{
    for (auto bs : busStationsSet)
    {
        if (bs->hasId(busStationId)) return bs;
    }
    return nullptr;
}

