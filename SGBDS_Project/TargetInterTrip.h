#pragma once
#include <iostream>
#include "BusStation.h"

using namespace std;

class TargetInterTrip
{

public:
    const BusStation *target;
    string interTripId;
    int duree;

public:

    TargetInterTrip(){

    }
    TargetInterTrip(const BusStation *target, string interTripId, int duree) : target(target)
    {
        this->duree = duree;
        this->interTripId = interTripId;
        this->target = target;
    }

    void showTarget() const
    {
        cout << "Duree: " << this->duree << ", " << this->interTripId << endl;
        this->target->showBusStation();
    }

    bool operator==(const TargetInterTrip &tip) const
    {
        return this->interTripId == tip.interTripId;
    }

    bool operator==(const TargetInterTrip *tip) const
    {
        return this->interTripId == tip->interTripId;
    }
    bool operator<(const TargetInterTrip &tip) const
    {
        return this->interTripId < tip.interTripId;
    }
    bool operator<(const TargetInterTrip *tip) const
    {
        return this->interTripId < tip->interTripId;
    }
};