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
        this->target->showBusStation();
        cout << " | " << this->duree; 
    }

    int getDuree() const 
    {
        return this->duree; 
    }

    static std::set<TargetInterTrip>::iterator findByTargetId(string targetId, set<TargetInterTrip> *targetsSet)
    {
        BusStation *bs = new BusStation(targetId, 0); 
        return targetsSet->find(TargetInterTrip(bs, "InterTripXX", 12)); 
    }

    bool operator==(const TargetInterTrip &tip) const
    {
        cout << endl << "using comp" << endl; 
        return *(this->target) == *(tip.target);
    }

    bool operator==(const TargetInterTrip *tip) const
    {
        cout << endl << "using comp" << endl; 
        return *(this->target) == *(tip->target);
    }
    bool operator<(const TargetInterTrip &tip) const
    {
        return *(this->target) < *(tip.target);
    }
    bool operator<(const TargetInterTrip *tip) const
    {
        return *(this->target) < *(tip->target);
    }
};