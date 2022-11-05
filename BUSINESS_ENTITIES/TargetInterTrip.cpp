#include "TargetInterTrip.h"

TargetInterTrip::TargetInterTrip()
{
}
TargetInterTrip::TargetInterTrip(const BusStation *target, string interTripId, int duree) : target(target)
{
    this->duree = duree;
    this->interTripId = interTripId;
    this->target = target;
}

void TargetInterTrip::showTarget() const
{
    this->target->showBusStation();
    cout << " | " << this->duree;
}

int TargetInterTrip::getDuree() const
{
    return this->duree;
}

int TargetInterTrip::findDurationByTargetId(string targetId, set<TargetInterTrip> *targetsSet)
{
    BusStation *bs = new BusStation(targetId, 0);
    std::set<TargetInterTrip>::iterator it = targetsSet->find(TargetInterTrip(bs, "InterTripXX", 12));
    if (it == targetsSet->end())
        return -1;
    return (*it).duree;
}

bool TargetInterTrip::operator==(const TargetInterTrip &tip) const
{
    cout << endl
         << "using comp" << endl;
    return *(this->target) == *(tip.target);
}

bool TargetInterTrip::operator==(const TargetInterTrip *tip) const
{
    cout << endl
         << "using comp" << endl;
    return *(this->target) == *(tip->target);
}
bool TargetInterTrip::operator<(const TargetInterTrip &tip) const
{
    return *(this->target) < *(tip.target);
}
bool TargetInterTrip::operator<(const TargetInterTrip *tip) const
{
    return *(this->target) < *(tip->target);
}
string TargetInterTrip::getInterTripId()
{
    return this->interTripId;
}
int TargetInterTrip::getDuree()
{
    return this->duree;
}
const BusStation *TargetInterTrip::getTarget() const
{
    return this->target;
}