#include <iostream>
#include <string>
#include "BusLine.hpp"
#include "BusStation.hpp"
#include "TripDate.hpp"
#include <ctime>

using namespace std;

class BusTrip
{
private:
    string tripId;
    BusStation *busStationDep;
    BusStation *busStationArr;
    TripDate *dateDepart;
    TripDate *dateArrivee;

public:
    BusTrip(
        string tripId,
        BusStation *busStationDep,
        BusStation *busStationArr,
        TripDate *dateDepart,
        TripDate *dateArrivee)
    {

        this->tripId = tripId;
        this->busStationDep = busStationDep;
        this->busStationArr = busStationArr;
        this->dateArrivee = dateArrivee;
        this->dateDepart = dateDepart;
    }

    // bool operator<(const BusTrip &trip)
    // {
    //     return ((this->dateArrivee - this->dateDepart) < (trip.dateArrivee - trip.dateDepart))
    // }
};