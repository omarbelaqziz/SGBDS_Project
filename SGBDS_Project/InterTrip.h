#pragma once
#include <string>
#include "BusStation.h"

class InterTrip
{
public:
	string id;
	const BusStation *departureBusStation;
	const BusStation *arrivalBusStation;
	int duration;

	InterTrip()
	{
	}

	InterTrip(string id, const BusStation *depBusSt, const BusStation *arrBusSt, int dur) : departureBusStation(depBusSt), arrivalBusStation(arrBusSt)
	{
		this->id = id;
		this->departureBusStation = depBusSt;
		this->arrivalBusStation = arrBusSt;
		this->duration = duration;
	}

	void showInterTrips()
	{
		cout << endl;
		cout << "InterTrip Id: " << this->id << endl;
		cout << "Departure station: " << this->departureBusStation << endl;
		cout << "Arrival station: " << this->arrivalBusStation << endl;
	}

	bool operator<(const InterTrip &inter_trip) const
	{
		return this->id < inter_trip.id;
	}
};
