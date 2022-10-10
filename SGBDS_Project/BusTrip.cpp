#include "BusTrip.h"
#include <iterator>

BusTrip::BusTrip(
    string tripId,
    const BusStation *busStationDep,
    const BusStation *busStationArr,
    string strDDep,
    string strDDarr,
    time_t dateDepart,
    time_t dateArrivee) : busStationDep(busStationDep), busStationArr(busStationArr)
{

    this->tripId = tripId;
    this->busStationDep = busStationDep;
    this->busStationArr = busStationArr;
    this->strDDep = strDDep; 
    this->strDDarr = strDDarr; 
    this->dateArr = dateArrivee;
    this->dateDep = dateDepart;
}

bool BusTrip::operator<(const BusTrip &trip) const
{
    return difftime(trip.dateDep, this->dateDep) > 0;
}

bool BusTrip::operator<(const BusTrip *trip) const
{
    return difftime(trip->dateDep, this->dateDep) > 0;
}

bool BusTrip::operator==(const BusTrip *bus) const
{
    return this->tripId.compare(bus->tripId); 
}
bool BusTrip::operator==(const BusTrip &bus) const
{
    return this->tripId == bus.tripId; 
}

void BusTrip::showBusTrip() const 
{
    cout << endl;
    cout << this->tripId << " : " ;
    busStationDep->showBusStation();
    cout << "@ " << this->strDDep << " -> "; 
    busStationArr->showBusStation(); 
    cout << "@ " << this->strDDarr; 

    cout << " | " << difftime(this->dateArr, this->dateDep) / 60 << endl;
}