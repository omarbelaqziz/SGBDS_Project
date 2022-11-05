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
    cout << this->tripId << " : ";
    busStationDep->showBusStation();
    cout << "@ " << this->strDDep << " -> ";
    busStationArr->showBusStation();
    cout << "@ " << this->strDDarr;

    cout << " | " << difftime(this->dateArr, this->dateDep) / 60 << endl;
}

// getters
int BusTrip::getTripDuration() const
{
    return difftime(this->getDateArr(), this->getDateDep()) / 60;
}
const BusStation *BusTrip::getBusStationDep() const
{
    return this->busStationDep;
}
const BusStation *BusTrip::getBusStationArr() const
{
    return this->busStationArr;
}

time_t BusTrip::getDateDep() const
{
    return this->dateDep;
}
time_t BusTrip::getDateArr() const
{
    return this->dateArr;
}
string BusTrip::getTripId() const
{
    return this->tripId;
}

// setters
void BusTrip::setBusStationDep(const BusStation *statDep)
{
    this->busStationDep = statDep;
}

void BusTrip::setBusStationArr(const BusStation *statArr)
{
    this->busStationArr = statArr;
}

void BusTrip::setDateDep(time_t dateDep)
{
    this->dateDep = dateDep;
}
void BusTrip::setDateArr(time_t dateArr)
{
    this->dateArr = dateArr;
}
void BusTrip::setTripId(string tripId)
{
    this->tripId = tripId;
}

void BusTrip::setStrDDep(string strDDep)
{
    this->strDDep = strDDep;
}
void BusTrip::setStrDDarr(string strDDarr)
{
    this->strDDarr = strDDarr;
}