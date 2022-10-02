#pragma once
#include <string>
#include "BusStation.hpp"

class InterTrip
{
	string id;
	BusStation* departureBusStation;
	BusStation* arrivalBusStation;
	int duration;
};

