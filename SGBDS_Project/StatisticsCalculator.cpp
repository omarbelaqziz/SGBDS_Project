#include "StatisticsCalculator.h"
#include <ctime>
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
	int dureeMin = 0;
	int dureeMax = 0;
	BusTrip* btMin = nullptr;
	BusTrip* btMax = nullptr;
	int somme = 0;

	for (auto p : map)
	{
		for (auto it = p.second->begin(); it != p.second->end(); ++it)
		{
			int duree = difftime((*it).getDateDep(), (*it).getDateDep()) / 60;
			somme += duree;
			if (btMin == nullptr || duree < dureeMin) {
				dureeMin = duree;
				btMin = &(*it);
			}
			if (btMax == nullptr || duree > dureeMax) {
				dureeMax = duree;
				btMax = &(*it);
			}

			cout << "Duree du voyage de " << (*it).getBusStationDep()->getId();
			cout << " vers " << (*it).getBusStationArr()->getId() << " est : ";
			cout << duree << "minutes" << endl;
		}
	}
	cout << " le voyage avec la duree minimal de : " << dureeMin << " minutes est " << btMin->getTripId() << endl;
	cout << " le voyage avec la duree maximal de : " << dureeMax << " minutes est " << btMax->getTripId() << endl;
	cout << " la moyenne des durees est : " << somme / tripsTotalNumber(map); << endl;
}

