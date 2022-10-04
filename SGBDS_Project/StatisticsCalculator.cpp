#include "StatisticsCalculator.h"
#include <ctime>
#include<iterator>

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
	const BusTrip* btMin = nullptr;
	const BusTrip* btMax = nullptr;
	int somme = 0;



	for (auto p : map)
	{
		multiset<BusTrip> ::iterator it; 
		for (it = p.second->begin(); it != p.second->end(); ++it)
		{
			int duree = difftime((*it).dateArr, (*it).dateDep) / 60;
			somme += duree;
			if (btMin == nullptr || duree < dureeMin) {
				dureeMin = duree;
				btMin = &(*it);
			}
			if (btMax == nullptr || duree > dureeMax) {
				dureeMax = duree;
				btMax = &(*it);
			}

			cout << "Duree du voyage " << it->getTripId() << " \tde " << (*it).getBusStationDep()->getId();
			cout << " \tvers " << (*it).getBusStationArr()->getId() << "\t est : ";
			cout << duree << " minutes" << endl;
		}
	}
	cout << " le voyage avec la duree minimal de : " << dureeMin << " minutes est " << btMin->getTripId() << endl;
	cout << " le voyage avec la duree maximal de : " << dureeMax << " minutes est " << btMax->getTripId() << endl;
	cout << " la moyenne des durees est : " << somme / tripsTotalNumber(map) << endl;
}

void showTripsNumberByLines(multiset<LineTrips> tripsNumbers)
{
	multiset<LineTrips> ::iterator it;
	for (it = tripsNumbers.begin(); it != tripsNumbers.end(); ++it)
	{
		cout << "Le nombre de voyeges de la ligne : " << (*it).idLine << " \test : " << it->nombreVoygs << endl;
 	}
}

void showAllStatistics(unordered_map<string, multiset<BusTrip>*> map)
{
	cout << "=================================================================================" << endl;
	cout << "### le nombre de voyages total est : " << tripsTotalNumber(map) << endl;
	cout << "=================================================================================" << endl;
	cout << "### le nombre des voyages par chaque ligne : " << endl;
	showTripsNumberByLines(tripsNumberByLines(map));
	cout << "=================================================================================" << endl;
	cout << "### les durees des voyages :" << endl;
	showDurations(map);
	cout << "=================================================================================" << endl;
}

