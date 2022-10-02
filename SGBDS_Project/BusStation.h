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
    BusStation(string id, bool isDepot);
    bool operator<(const BusStation &bus);
    bool operator<(const BusStation *bus);
    void showBusStation();
    bool hasId(string stationId);
};
