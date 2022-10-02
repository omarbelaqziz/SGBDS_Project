#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class BusStation
{
private:
    string id;
    bool isDepot;

public:
    BusStation(string id, bool isDepot)
    {
        this->id = id;
        this->isDepot = isDepot;
    }
    bool operator<(const BusStation &bus)
    {
        return this->id < bus.id;
    }

    bool operator<(const BusStation *bus)
    {
        return this->id < bus->id;
    }

    void showBusStation()
    {
        cout << endl;
        cout << "Station Id: " << this->id << endl;
        cout << "Is Depot: " << this->isDepot << endl;
    }
};
