#include "BusTrip.h"
#include <iterator>

BusTrip::BusTrip(
    string tripId,
    const BusStation *busStationDep,
    const BusStation *busStationArr,
    time_t dateDepart,
    time_t dateArrivee) : busStationDep(busStationDep), busStationArr(busStationArr)
{

    this->tripId = tripId;
    this->busStationDep = busStationDep;
    this->busStationArr = busStationArr;
    this->dateArr = dateArrivee;
    this->dateDep = dateDepart;
}

bool BusTrip::operator<(const BusTrip &trip) const
{
    return (difftime(this->dateArr, this->dateDep)) < (difftime(trip.dateArr, trip.dateDep));
}

bool BusTrip::operator<(const BusTrip *trip) const
{
    return (difftime(this->dateArr, this->dateDep)) < (difftime(trip->dateArr, trip->dateDep));
}

bool BusTrip::operator==(const BusTrip *bus) const
{
    return this->tripId.compare(bus->tripId); 
}
bool BusTrip::operator==(const BusTrip &bus) const
{
    return this->tripId == bus.tripId; 
}

void BusTrip::showBusTrip()
{
    cout << endl;
    cout << "tripId: " << this->tripId << endl;
    busStationArr->showBusStation();
    busStationDep->showBusStation();
    cout << "trip Duration: " << difftime(this->dateArr, this->dateDep) / 60 << endl;
}