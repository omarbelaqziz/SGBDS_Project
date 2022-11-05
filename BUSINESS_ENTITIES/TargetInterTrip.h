#pragma once
#include <iostream>
#include <set> 
#include "BusStation.h"


using namespace std;

class TargetInterTrip
{

private:
    const BusStation *target;
    string interTripId;
    int duree;

public:
    TargetInterTrip();
    TargetInterTrip(const BusStation *target, string interTripId, int duree);

    void showTarget() const;

    int getDuree() const;

    static int findDurationByTargetId(string targetId, set<TargetInterTrip> *targetsSet);

    bool operator==(const TargetInterTrip &tip) const;
    bool operator==(const TargetInterTrip *tip) const;
    bool operator<(const TargetInterTrip &tip) const;
    bool operator<(const TargetInterTrip *tip) const;
    string getInterTripId();
    int getDuree();
    const BusStation *getTarget() const;
};