#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class BusStation
{
public:
    string id;
    bool isDepot;

public:
    BusStation(string id, bool isDepot);
    bool operator<(const BusStation &bus) const;
    bool operator<(const BusStation *bus) const;
    bool operator==(const BusStation *bus) const;
    bool operator==(const BusStation &bus) const;

    void showBusStation() const;
    bool hasId(string stationId);

    string getId() const ;
    bool getIsDepot() const ;
};
