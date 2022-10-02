#include "BusTrip.h"

BusTrip::BusTrip(
    string tripId,
    BusStation *busStationDep,
    BusStation *busStationArr,
    time_t dateDep,
    time_t dateArr)
{

    this->tripId = tripId;
    this->busStationDep = busStationDep;
    this->busStationArr = busStationArr;
    this->dateArr = dateArr;
    this->dateDep = dateDep;
}

bool BusTrip::operator<(const BusTrip& trip)
{
   return (difftime(this->dateArr, this->dateDep)) < (difftime(trip.dateArr, trip.dateDep));
}