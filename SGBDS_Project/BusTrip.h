#include <iostream>
#include <string>
#include "BusLine.h"
#include "BusStation.h"
#include "TripDate.h"
#include <ctime>
#include <cstdlib>

#pragma once

using namespace std;

class BusTrip
{
private:
    string tripId;
    BusStation *busStationDep;
    BusStation *busStationArr;
    time_t dateDep; 
    time_t dateArr; 

public:
    BusTrip(
        string tripId,
        BusStation *busStationDep,
        BusStation *busStationArr,
        time_t dateDepart,
        time_t dateArrivee);

    bool operator<(const BusTrip& trip);
    // {
    //     return ((this->dateArrivee - this->dateDepart) < (trip.dateArrivee - trip.dateDepart))
    // }
};