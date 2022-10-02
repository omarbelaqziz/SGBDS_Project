#include "BusTrip.h"

BusTrip::BusTrip(
    string tripId,
    BusStation *busStationDep,
    BusStation *busStationArr,
    TripDate *dateDepart,
    TripDate *dateArrivee)
{

    this->tripId = tripId;
    this->busStationDep = busStationDep;
    this->busStationArr = busStationArr;
    this->dateArrivee = dateArrivee;
    this->dateDepart = dateDepart;
}