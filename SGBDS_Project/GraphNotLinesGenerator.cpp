#include "GraphNotLinesGenerator.h"
#include "BusStationsHandler.h"
#include <list>

typedef struct
{
    int duree_total_depot;
    int duree_total_attente;
    int duree_total_hlp;

    int somme_hlp;
    int somme_covered_voyages;
    int somme_clusters;
    int clusters_with_one_trip;
    double cout_total_depot;
} DepotStats;

void detach_lines(multiset<BusTrip> &resultTrips, TRIPS_MAP linesTrips)
{
    TRIPS_MAP ::iterator it1;
    for (it1 = linesTrips.begin(); it1 != linesTrips.end(); ++it1)
    {
        multiset<BusTrip>::iterator temp_it;
        for (temp_it = (*it1).second->begin(); temp_it != (*it1).second->end(); ++temp_it)
        {
            resultTrips.insert((*temp_it));
        }
    }
}

void heuristic_graph_builder(multiset<BusTrip> busTripsPopulation, ofstream &output_file, set<BusStation> *busStations, INTER_TRIPS stationsTargets)
{

    DepotStats depot_stats;
    depot_stats.duree_total_depot = 0;
    depot_stats.duree_total_attente = 0;
    depot_stats.duree_total_hlp = 0;
    depot_stats.somme_hlp = 0;
    depot_stats.somme_covered_voyages = 0;
    depot_stats.somme_clusters = 0;
    depot_stats.clusters_with_one_trip=0;
    depot_stats.cout_total_depot = 0.0;

    int population_size = busTripsPopulation.size();
    int res = 0;
    bool isTreated;
    int clusterCount = 0;

    int its = 0;

    // copy the data to a temp set of pair<BusTrip, isVisited>
    pair<BusTrip, bool> *temp;
    pair<BusTrip, bool> *ptr1;
    pair<BusTrip, bool> *ptr2;

    temp = new pair<BusTrip, bool>[busTripsPopulation.size()];

    multiset<BusTrip>::iterator temp_it;

    int u = 0;
    for (temp_it = busTripsPopulation.begin(); temp_it != busTripsPopulation.end(); ++temp_it)
    {
        (temp + u)->first = (*temp_it);
        (temp + u)->second = false;
        u++;
    }
    string depotId = findDepotId(busStations);

    while (res != population_size)
    {
        int duree_total = 0;
        int duree_attente = 0;
        int duree_hlp = 0;
        int hlp_number = 0;
        double cout_total = 0;
        isTreated = false;
        clusterCount++;
        int number_trips = 0;

        its++;

        output_file << "-----------------------" << endl;
        output_file << "Cluster: " << clusterCount << endl;
        output_file << "Depot --> ";

        ptr1 = temp;

        while ((*ptr1).second != false)
            ptr1++;

        // add from depot duration

        duree_total += TargetInterTrip::findDurationByTargetId((*ptr1).first.busStationDep->getId(), stationsTargets[depotId]);
        if (duree_total == -1)
        {
            cout << "duration between depot and " << (*ptr1).first.busStationDep->getId() << " not found" << endl;
            exit(-1);
        }
        cout_total += duree_total * c_v;

        if (ptr1 == (temp + population_size - 1))
        {
            int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
            if (duree == -1)
            {
                cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                exit(-1);
            }
            duree_total += duree;
            cout_total += duree * c_v;
            (*ptr1).second = true;
            res++;
            depot_stats.somme_covered_voyages++;
            output_file << (*ptr1).first.tripId << "--> Depot" << endl;
            number_trips++;
            isTreated = true;
            duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
        }
        else
        {
            ptr2 = ptr1;
            while (!isTreated)
            {

                while ((*ptr2).second && ptr2 != (temp + population_size - 1))
                {
                    ptr2++;
                }
                if (ptr2 == (temp + population_size - 1))
                {
                    int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                    if (duree == -1)
                    {
                        cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                        exit(-1);
                    }
                    duree_total += duree;
                    cout_total += duree * c_v;
                    (*ptr1).second = true;
                    res++;
                    output_file << (*ptr1).first.tripId << "--> Depot" << endl;
                    number_trips++;
                    isTreated = true;
                    depot_stats.somme_covered_voyages++;
                    duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                }
                else
                {
                    if ((*ptr2).first.busStationDep == (*ptr1).first.busStationArr)
                    {
                        if (0 <= difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 &&
                            difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 <= 45)
                        {
                            output_file << (*ptr1).first.tripId << "--> waitInStation(" << difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 << ") --->";
                            number_trips++;
                            (*ptr1).second = true;
                            res++;
                            depot_stats.somme_covered_voyages++;
                            duree_attente += difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60;
                            duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60 + difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60);
                            ptr1 = ptr2;
                            ptr2++;
                        }
                        // new cluster
                        else if (difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 > 45)
                        {
                            int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                            if (duree == -1)
                            {
                                cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                                exit(-1);
                            }
                            duree_total += duree;
                            cout_total += duree * c_v;
                            output_file << (*ptr1).first.tripId << " ---> Depot" << endl;
                            number_trips++;
                            (*ptr1).second = true;
                            // added by omar <=>
                            // we need to add trip duration even if he had to go to depot
                            duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                            // <=>
                            res++;
                            isTreated = true;
                            depot_stats.somme_covered_voyages++;
                        }
                        else
                        {
                            ptr2++;
                        }
                    }
                    else
                    {
                        auto PTRR = (*ptr2).first.busStationDep;

                        if (PTRR == NULL)
                        {
                            cout << endl
                                 << "---> There is no no data for ptr2 :: " << endl;
                            exit(-1);
                        }
                        else
                        {
                            int duree = TargetInterTrip::findDurationByTargetId((*ptr2).first.busStationDep->getId(), stationsTargets[(*ptr1).first.busStationArr->getId()]);

                            if (duree == -1)
                            {

                                // SEARCH IN THE NEGATIVE WAY
                                int temp_duree = TargetInterTrip::findDurationByTargetId((*ptr1).first.busStationArr->getId(), stationsTargets[(*ptr2).first.busStationDep->getId()]);
                                if (temp_duree == -1)
                                {
                                    cout << endl
                                         << "---> There is no InterTrip between " << (*ptr1).first.busStationArr->id << " -> " << (*ptr2).first.busStationDep->getId() << " <-- " << endl;
                                    exit(-1);
                                }
                                else
                                {
                                    duree = temp_duree;
                                }
                            }
                            int attente = (difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60) - duree;

                            if (attente > 0)
                            {
                                if (attente > 45)
                                {
                                    // added by omar <=>
                                    // we need to add trip duration even if he had to go to depot
                                    duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                                    // <=>
                                    output_file << (*ptr1).first.tripId << " --> Depot" << endl;
                                    number_trips++;
                                    (*ptr1).second = true;
                                    res++;
                                    isTreated = true;
                                    depot_stats.somme_covered_voyages++;
                                }
                                else
                                {
                                    duree_attente += attente;
                                    duree_hlp += duree;
                                    duree_total += attente + duree;
                                    hlp_number++;
                                    output_file << (*ptr1).first.tripId << " -- HLP --> "
                                                << " waitInStation (" << attente << ")"
                                                << " -->";
                                    number_trips++;
                                    depot_stats.somme_covered_voyages++;
                                    (*ptr1).second = true;
                                    res++;
                                    ptr1 = ptr2;
                                    ptr2++;
                                }
                            }
                            else
                            {
                                ptr2++;
                            }
                        }
                    }
                }
            }
        }

        output_file << " __________________________________________________________________________________________________ " << endl;
        output_file << "|#Cluster|Duree Total|Cout total|Nombre HLP|Duree HLP|% HLP|Duree Attente|% Attente|Nombre de trips" << endl;

        cout_total += c_a * duree_attente + c_v * duree_hlp;
        output_file << "|" << setw(8) << clusterCount << "|" << setw(7) << duree_total << " min|" << setw(10) << cout_total << "|" << setw(6) << hlp_number << " HLP|" 
            << setw(5) << duree_hlp << " min|" << setw(3) << duree_hlp * 100 / duree_total << " %|" << setw(9) << duree_attente << " min|" << setw(7) << duree_attente * 100 / duree_total << " %|" << number_trips << endl;
        output_file << " ---------------------------------------------------------------------------------- " << endl;

        // collecting informations
        depot_stats.duree_total_attente += duree_attente;
        depot_stats.duree_total_depot += duree_total;
        depot_stats.duree_total_hlp += duree_hlp;
        depot_stats.cout_total_depot += cout_total;
        depot_stats.somme_hlp += hlp_number;
        //collecting informations for number of clusters with one trip
        if (number_trips == 1)
            depot_stats.clusters_with_one_trip++;

        // collect also the total of trips and clusters

        duree_total = 0;
        duree_attente = 0;
        duree_hlp = 0;
    }

    depot_stats.somme_clusters += its;

    output_file << "-------------" << endl;
    output_file << "Finished all Line Trips" << endl;
    output_file << "-------------" << endl;
    cout << "N# clusters: " << its << endl;
    cout << "Cout de depot; " << depotId << " : " << depot_stats.cout_total_depot << endl;

    cout << " ___________________________________________________________________________________________________________________________ " << endl;
    cout << "|#Depot  |Duree Total|Cout total|Total HLP|Duree HLP|% HLP|Total Attente|% Attente|Somme Clusters|Covered Trips|Clusters with one trip" << endl;

    cout << "|" << setw(8) << depotId << "|" << setw(7) << depot_stats.duree_total_depot << " min|" << setw(10) << depot_stats.cout_total_depot << "|" 
        << setw(6) << depot_stats.duree_total_hlp << " HLP|" << setw(5) << depot_stats.somme_hlp << " min|" 
        << setw(3) << depot_stats.duree_total_hlp * 100 / depot_stats.duree_total_depot << " %|" 
        << setw(9) << depot_stats.duree_total_attente << " min|" << setw(7) << depot_stats.duree_total_attente * 100 / depot_stats.duree_total_depot 
        << " %|" << setw(7)<< depot_stats.somme_clusters << "|" << setw(10) << depot_stats.somme_covered_voyages <<"|" <<setw(7)<<depot_stats.clusters_with_one_trip << endl;
    cout << " ---------------------------------------------------------------------------------- " << endl;
}