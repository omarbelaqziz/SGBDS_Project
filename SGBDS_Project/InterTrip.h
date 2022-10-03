#pragma once
#include <string>
#include "BusStation.h"

class InterTrip
{
	string id;
	BusStation* departureBusStation;
	BusStation* arrivalBusStation;
	int duration;
public:
	InterTrip(string id, BusStation* departureBusStation, BusStation* arrivalBusStation, int duration);
	void showInterTrips();
};

