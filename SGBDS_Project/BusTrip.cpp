#include "BusTrip.h"

BusTrip::BusTrip(
    string tripId,
    BusStation *busStationDep,
    BusStation *busStationArr,
    time_t dateDepart,
    time_t dateArrivee)
{

    this->tripId = tripId;
    this->busStationDep = busStationDep;
    this->busStationArr = busStationArr;
    this->dateArr = dateArrivee;
    this->dateDep = dateDepart;
}

bool BusTrip::operator<(const BusTrip &trip)
{
    return (difftime(this->dateArr, this->dateDep)) < (difftime(trip.dateArr, trip.dateDep));
}

bool BusTrip::operator<(const BusTrip *trip)
{
    return (difftime(this->dateArr, this->dateDep)) < (difftime(trip->dateArr, trip->dateDep));
}

bool BusTrip::operator==(const BusTrip *bus)
{
    return this->tripId == bus->tripId; 
}
bool BusTrip::operator==(const BusTrip &bus)
{
    return this->tripId == bus.tripId; 
}

void BusTrip::showBusTrip()
{
    cout << endl;
    cout << "tripId: " << this->tripId << endl;
    // busStationArr->showBusStation();
    // busStationDep->showBusStation();
    // cout << "date ARR: " << this->dateArr << endl;
    // cout << "date Dep: " << this->dateDep << endl;
}