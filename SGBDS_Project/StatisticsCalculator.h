#pragma once
#include <unordered_map>
#include <set>
#include <string>
#include "BusTrip.h"

using namespace std;
struct LineTrips {
	string idLine;
	int nombreVoygs;
	inline bool operator <(const LineTrips& lt) const {
		return nombreVoygs < lt.nombreVoygs;
	}
};







int tripsTotalNumber(unordered_map<string, multiset<BusTrip>*> map);
multiset<LineTrips> tripsNumberByLines(unordered_map<string, multiset<BusTrip>*> map);
void showDurations(unordered_map<string, multiset<BusTrip>*> map);
void showTripsNumberByLines(multiset<LineTrips> tripsNumbers);
void showAllStatistics(unordered_map<string, multiset<BusTrip>*> map);