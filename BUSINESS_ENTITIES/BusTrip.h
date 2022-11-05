#pragma once
#include <iostream>
#include <string>
#include "BusStation.h"
#include <ctime>
#include <cstdlib>

#pragma once

using namespace std;

class BusTrip
{
private:
    string tripId;
    const BusStation *busStationDep;
    const BusStation *busStationArr;
    string strDDep;
    string strDDarr;
    time_t dateDep;
    time_t dateArr;

public:
    BusTrip()
    {
    }

    BusTrip(
        string tripId,
        const BusStation *busStationDep,
        const BusStation *busStationArr,
        string strDDep,
        string strDDarr,
        time_t dateDepart,
        time_t dateArrivee);

    bool operator<(const BusTrip &trip) const;
    bool operator<(const BusTrip *trip) const;
    bool operator==(const BusTrip *bus) const;
    bool operator==(const BusTrip &bus) const;
    void showBusTrip() const;

    // getters
    int getTripDuration() const;
    const BusStation *getBusStationDep() const;
    const BusStation *getBusStationArr() const;
    time_t getDateDep() const;
    time_t getDateArr() const;
    string getTripId() const;

    // setters
    void setBusStationDep(const BusStation* statDep);
    void setBusStationArr(const BusStation* statArr);
    void setDateDep(time_t dateDep);
    void setDateArr(time_t dateArr);
    void setTripId(string tripId);
    void setStrDDep(string strDDep);
    void setStrDDarr(string strDDarr);
};