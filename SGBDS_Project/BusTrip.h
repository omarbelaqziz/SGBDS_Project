#include <iostream>
#include <string>
#include "BusLine.h"
#include "BusStation.h"
#include <ctime>
#include <cstdlib>

#pragma once

using namespace std;

class BusTrip
{
public:
    string tripId;
    const BusStation* busStationDep;
    const BusStation *busStationArr;
    time_t dateDep;
    time_t dateArr;

public:

    BusTrip() {

    }

    BusTrip(
        string tripId,
        const BusStation *busStationDep,
        const BusStation *busStationArr,
        time_t dateDepart,
        time_t dateArrivee);

    bool operator<(const BusTrip &trip) const;
    bool operator<(const BusTrip *trip) const;
    bool operator==(const BusTrip *bus) const;
    bool operator==(const BusTrip &bus) const;
    void showBusTrip();
    const BusStation* getBusStationDep() {
        return this->busStationDep;
    }
    const BusStation* getBusStationArr() {
        return this->busStationArr;
    }

    time_t getDateDep() {
        return this->dateDep;
    }
    time_t getDateArr() {
        return this->dateArr;
    }
    string getTripId() {
        return this->tripId;
    }
};