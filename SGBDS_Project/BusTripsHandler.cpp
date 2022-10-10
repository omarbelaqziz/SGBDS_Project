#include "BusTripsHandler.h"
#include "StringsOperations.h"
#include <iterator>

void handle_file_stream_bus_trips(unordered_map<string, multiset<BusTrip> *> &tripsMap, ifstream &i_file, set<BusStation> *busStationsSet)
{
    string line;
    vector<string> rawData;

    while (getline(i_file, line))
    {
        if (line.find("}") != string::npos)
        {
            break;
        }
        rawData = StringsOperations::split(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
        BusTrip busTrip;
        buildBusTrip(busTrip, rawData, *busStationsSet);
        multiset<BusTrip> *BSTrips;
        if (tripsMap.count(rawData[0]) > 0)
        {
            BSTrips = tripsMap[rawData[0]];
            BSTrips->insert(busTrip);
        }
        else
        {
            BSTrips = new multiset<BusTrip>();
            BSTrips->insert(busTrip);
            tripsMap[rawData[0]] = BSTrips;
        }
    }
}

void buildBusTrip(BusTrip &busTrip, vector<string> rawData, set<BusStation> &busStationsSet)
{
    auto depBusStation = busStationsSet.find(BusStation(rawData[2], false));

    auto arrivalBusStation = busStationsSet.find(BusStation(rawData[5], false));
    time_t depTime = dateTimeStringToTimeObject(rawData[3], rawData[4]);
    time_t arrivalTime = dateTimeStringToTimeObject(rawData[6], rawData[7]);

    if(rawData.size() != 8)
    {
        cout << "--> this vector of trips have a syntaxique problem : " ;
        for(auto v: rawData) {
            cout << v << ", ";
        } 
        cout << endl; 
        exit(-1); 
    }
    if (depBusStation != busStationsSet.end() && arrivalBusStation != busStationsSet.end())
    {
        busTrip.tripId = rawData[1],
        busTrip.busStationDep = &(*depBusStation),
        busTrip.busStationArr = &(*arrivalBusStation),
        busTrip.strDDarr = rawData[7]; 
        busTrip.strDDep = rawData[4]; 
        busTrip.dateDep = depTime,
        busTrip.dateArr = arrivalTime;
    }

    else
    {
        cout << " --> One of the stations : " << rawData[7] << " || " << rawData[4] << " is not in the busStations set <--" << endl;
        exit(-1);    
    }
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

BusStation *findBusStationById(const set<BusStation *> &busStationsSet, string busStationId)
{
    for (auto bs : busStationsSet)
    {
        if (bs->hasId(busStationId))
            return bs;
    }
    return nullptr;
}
