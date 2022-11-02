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

    int getTripDuration() {
        return difftime(this->getDateArr(), this->getDateDep())/60; 
    }
    const BusStation *getBusStationDep() const
    {
        return this->busStationDep;
    }
    const BusStation *getBusStationArr() const
    {
        return this->busStationArr;
    }

    time_t getDateDep() const
    {
        return this->dateDep;
    }
    time_t getDateArr() const
    {
        return this->dateArr;
    }
    string getTripId() const
    {
        return this->tripId;
    }
// setters

    void setBusStationDep(const BusStation* statDep)
    {
        this->busStationDep = statDep;
    }

    void setBusStationArr(const BusStation* statArr)
    {
        this->busStationArr = statArr;
    }
    
    void setDateDep(time_t dateDep) 
    {
        this->dateDep = dateDep;
    }
    void setDateArr(time_t dateArr)
    {
        this->dateArr = dateArr;
    }
    void setTripId(string tripId)
    {
        this->tripId = tripId;
    }

    void setStrDDep(string strDDep)
    {
        this->strDDep = strDDep;
    }
    void setStrDDarr(string strDDarr)
    {
        this->strDDarr = strDDarr;
    }
    
};