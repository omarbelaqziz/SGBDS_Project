#include <iostream>
#include <string>
#include "BusLine.h"
#include "BusStation.h"
#include "TripDate.h"
#include <ctime>

using namespace std;

class BusTrip
{
private:
    string tripId;
    BusStation *busStationDep;
    BusStation *busStationArr;
    TripDate *dateDepart;
    TripDate *dateArrivee;

public:
    BusTrip(
        string tripId,
        BusStation *busStationDep,
        BusStation *busStationArr,
        TripDate *dateDepart,
        TripDate *dateArrivee);

    // bool operator<(const BusTrip &trip)
    // {
    //     return ((this->dateArrivee - this->dateDepart) < (trip.dateArrivee - trip.dateDepart))
    // }
};