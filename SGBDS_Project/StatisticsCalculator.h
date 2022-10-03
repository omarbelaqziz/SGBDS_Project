#pragma once
#include <unordered_map>
#include <set>
#include <string>
#include "BusTrip.h"

struct LineTrips {
	string idLine;
	int nombreVoygs;
	bool operator <(const LineTrips& lt) const {
		return nombreVoygs < lt.nombreVoygs;
	}
};



using namespace std;



int tripsTotalNumber(unordered_map<string, multiset<BusTrip>*> map);
multiset<LineTrips> tripsNumberByLines(unordered_map<string, multiset<BusTrip>*> map);
void showDurations(unordered_map<string, multiset<BusTrip>*> map);

