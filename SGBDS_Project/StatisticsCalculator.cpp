#include "StatisticsCalculator.h"

int tripsTotalNumber(unordered_map<string, multiset<BusTrip>*> map)
{
    int total = 0;

	for (auto p : map)
	{
		total += p.second->size();
	}
    return total;
}

multiset<LineTrips> tripsNumberByLines(unordered_map<string, multiset<BusTrip>*> map)
{
	multiset<LineTrips> lineTrips;

	for (auto p : map)
	{
		struct LineTrips lt;
		lt.idLine = p.first;
		lt.nombreVoygs = p.second->size();
		lineTrips.insert(lt);
	}

	return lineTrips;
}

void showDurations(unordered_map<string, multiset<BusTrip>*> map)
{
	for (auto p : map)
	{
		for (auto it = p.second->begin(); it != p.second->end(); ++it)
		{
			//cout << "Duree du voyage de " << it.getBusStationDep().
		}
	}
}
