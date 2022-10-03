#include "InterTrip.h"

InterTrip::InterTrip(string id, BusStation* departureBusStation, BusStation* arrivalBusStation, int duration)
{
	this->id = id;
	this->departureBusStation = departureBusStation;
	this->arrivalBusStation = arrivalBusStation;
	this->duration = duration;
}

void InterTrip::showInterTrips()
{
	cout << endl;
	cout << "InterTrip Id: " << this->id << endl;
	cout << "Departure station: " << this->departureBusStation << endl;
	cout << "Arrival station: " << this->arrivalBusStation << endl;
}