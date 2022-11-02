#pragma once
#include <iostream>
#include "BusStation.h"

using namespace std;

class TargetInterTrip
{

private:
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

    static int findDurationByTargetId(string targetId, set<TargetInterTrip> *targetsSet)
    {
        BusStation *bs = new BusStation(targetId, 0); 
        std::set<TargetInterTrip>::iterator it = targetsSet->find(TargetInterTrip(bs, "InterTripXX", 12)); 
        if(it == targetsSet->end()) return -1; 
        return (*it).duree; 
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
    string getInterTripId(){
        return this->interTripId;
    }
    int getDuree(){
        return this->duree;
    }
    const BusStation * getTarget() const{
        return this->target;
    }
};