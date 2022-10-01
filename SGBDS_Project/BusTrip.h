#pragma once
#include <string>
#include <ctime>
#include "BusStation.h"

using namespace std;
class BusTrip
{
	string id;
	time_t departureDate;
	time_t arrivalDate;
	BusStation* departureBusStation;
	BusStation* arrivalBusStation;
};

