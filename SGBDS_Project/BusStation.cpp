#include "BusStation.h"

BusStation::BusStation(string id, bool isDepot)
{
    this->id = id;
    this->isDepot = isDepot;
}

bool BusStation::operator<(const BusStation &bus) const
{
    return this->id < bus.id;
}

bool BusStation::operator<(const BusStation *bus) const 
{
    return this->id < bus->id;
}

void BusStation::showBusStation() const
{
    cout << endl;
    cout << "Station Id: " << this->id << endl;
    cout << "Is Depot: " << this->isDepot << endl;
}
bool BusStation::operator==(const BusStation *bus) const 
{
    return this->id == bus->id;
}
bool BusStation::operator==(const BusStation &bus) const
{
    return this->id == bus.id;
}

bool BusStation::hasId(string stationId)
{
    return this->id == stationId;
}

string BusStation::getId()
{
    return this->id;
}

bool BusStation::getIsDepot()
{
    return this->isDepot;
}
