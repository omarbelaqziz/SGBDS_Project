#include "GraphGenerator.h"
#include "BusStationsHandler.h"
#include <list>
#include <vector>
#include <set>
#include <iomanip>

struct StatsItineraire
{
    int duree_total;
    double cout_total;
    int nbr_hlp;

    // a list that will contains a pair of the hlp duration with the
    // percentage due to the totla duration of the itineraire
    list<pair<int, double>> hlps;
};

typedef struct
{
    int duree_total_depot;
    int duree_total_attente;
    int duree_total_hlp;

    int somme_hlp;
    int somme_covered_voyages;
    int somme_clusters;

    double cout_total_depot;
} DepotStats;



void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips,
    ofstream &output_file)
{
    // initialisation des stats de depot

    DepotStats depot_stats;
    depot_stats.duree_total_depot = 0;
    depot_stats.duree_total_attente = 0;
    depot_stats.duree_total_hlp = 0;
    depot_stats.somme_hlp = 0;
    depot_stats.somme_covered_voyages = 0;
    depot_stats.somme_clusters = 0;
    depot_stats.cout_total_depot = 0.0;

    int its = 0;
    TRIPS_MAP::iterator global_iterator;
    int iii = 0;
    // handle each Line
    string depotId = findDepotId(busStations);
    if (depotId == "")
    {
        cout << "no depot found in data" << endl;
        exit(-1);
    }
    for (global_iterator = linesTrips.begin(); global_iterator != linesTrips.end(); ++global_iterator)
    {
        // show the Depot

        int res = 0;
        int clusterCount = 0;
        bool isTreated;

        // copy the data to a temp set of pair<BusTrip, isVisited>
        pair<BusTrip, bool> *temp;
        pair<BusTrip, bool> *ptr1;
        pair<BusTrip, bool> *ptr2;

        // temp = (pair<BusTrip, bool> *)malloc(sizeof(pair<BusTrip, bool>) * (*global_iterator).second->size());
        temp = new pair<BusTrip, bool>[(*global_iterator).second->size()];

        multiset<BusTrip>::iterator temp_it;

        int u = 0;
        for (temp_it = (*global_iterator).second->begin(); temp_it != (*global_iterator).second->end(); ++temp_it)
        {
            if (u == (*global_iterator).second->size())
                break;
            (temp + u)->first = (*temp_it);
            (temp + u)->second = false;
            u++;
        }

        iii++;
        // show Line
        output_file << endl
                    << "***************" << endl
                    << "Line: " << iii << " (" << (*global_iterator).first << " ) / " << linesTrips.size() << endl
                    << "***************" << endl;

        while (res != (*global_iterator).second->size())
        {
            int duree_total = 0;
            int duree_attente = 0;
            int duree_hlp = 0;
            int hlp_number = 0;
            double cout_total = 0;
            isTreated = false;
            clusterCount++;

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

            if (ptr1 == (temp + (*global_iterator).second->size() - 1))
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
                depot_stats.somme_covered_voyages ++; 
                output_file << (*ptr1).first.tripId << "--> Depot" << endl;
                isTreated = true;
                duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
            }
            else
            {
                ptr2 = ptr1;
                while (!isTreated)
                {

                    while ((*ptr2).second && ptr2 != (temp + (*global_iterator).second->size() - 1))
                    {
                        ptr2++;
                    }
                    if (ptr2 == (temp + (*global_iterator).second->size() - 1))
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
                        isTreated = true;
                        depot_stats.somme_covered_voyages ++; 
                        duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                    }
                    else
                    {
                        if ((*ptr2).first.busStationDep == (*ptr1).first.busStationArr)
                        {
                            if (0 <= difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 &&
                                difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 <= MIN_WAIT)
                            {
                                output_file << (*ptr1).first.tripId << "--> waitInStation(" << difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 << ") --->";
                                (*ptr1).second = true;
                                res++;

                                duree_attente += difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60;
                                duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60 + difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60);
                                ptr1 = ptr2;
                                ptr2++;
                            }
                            // new cluster
                            else if (difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 > MIN_WAIT)
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
                                (*ptr1).second = true;
                                // added by omar <=>
                                // we need to add trip duration even if he had to go to depot
                                duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                                // <=>
                                res++;
                                isTreated = true;
                                depot_stats.somme_covered_voyages ++; 
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
                                    if (attente > MIN_WAIT)
                                    {
                                        // added by omar <=>
                                        // we need to add trip duration even if he had to go to depot
                                        duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                                        // <=>
                                        output_file << (*ptr1).first.tripId << " --> Depot" << endl;
                                        (*ptr1).second = true;
                                        res++;
                                        isTreated = true;
                                        depot_stats.somme_covered_voyages ++; 
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

            output_file << " __________________________________________________________________________________ " << endl;
            output_file << "|#Cluster|Duree Total|Cout total|Nombre HLP|Duree HLP|% HLP|Duree Attente|% Attente|" << endl;

            cout_total += c_a * duree_attente + c_v * duree_hlp;
            output_file << "|" << setw(8) << clusterCount << "|" << setw(7) << duree_total << " min|" << setw(10) << cout_total << "|" << setw(6) << hlp_number << " HLP|" << setw(5) << duree_hlp << " min|" << setw(3) << duree_hlp * 100 / duree_total << " %|" << setw(9) << duree_attente << " min|" << setw(7) << duree_attente * 100 / duree_total << " %|" << endl;
            output_file << " ---------------------------------------------------------------------------------- " << endl;

            // collecting informations
            depot_stats.duree_total_attente += duree_attente;
            depot_stats.duree_total_depot += duree_total;
            depot_stats.duree_total_hlp += duree_hlp;
            depot_stats.cout_total_depot += cout_total;
            depot_stats.somme_hlp += hlp_number;

            // collect also the total of trips and clusters

            duree_total = 0;
            duree_attente = 0;
            duree_hlp = 0;
        }
    }
    depot_stats.somme_clusters += its;

    output_file << "-------------" << endl;
    output_file << "Finished all Line Trips" << endl;
    output_file << "-------------" << endl;
    cout << "N# clusters: " << its << endl;
    cout << "Cout de depot; " << depotId << " : " << depot_stats.cout_total_depot << endl;

    cout << " __________________________________________________________________________________ " << endl;
    cout << "|#Depot  |Duree Total|Cout total|Total HLP|Duree HLP|% HLP|Total Attente|% Attente|Somme Clusters|Covered Trips" << endl;

    cout << "|" << setw(8) << depotId << "|" << setw(7) << depot_stats.duree_total_depot << " min|" << setw(10) << depot_stats.cout_total_depot << "|" << setw(6) << depot_stats.duree_total_hlp << " HLP|" << setw(5) << depot_stats.somme_hlp << " min|" << setw(3) << depot_stats.duree_total_hlp * 100 / depot_stats.duree_total_depot << " %|" << setw(9) << depot_stats.duree_total_attente << " min|" << setw(7) << depot_stats.duree_total_attente * 100 / depot_stats.duree_total_depot << " %|" << depot_stats.somme_clusters << "|" << setw(7) << depot_stats.somme_covered_voyages  << endl;
    cout << " ---------------------------------------------------------------------------------- " << endl;
}



vector<vector<string>*> clusters_generator_fromTripsSet(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    multiset<BusTrip> trips,
    int mode)
    // mode 0 with durations
    // other with no duration
{
    
    vector<vector<string>*> allClusters;

    // handle each Line
    string depotId = findDepotId(busStations);
    if (depotId == "")
    {
        cout << "no depot found in data" << endl;
        exit(-1);
    }
        // show the Depot

        int res = 0;
        bool isTreated;

        // copy the data to a temp set of pair<BusTrip, isVisited>
        pair<BusTrip, bool> *temp;
        pair<BusTrip, bool> *ptr1;
        pair<BusTrip, bool> *ptr2;

        temp = new pair<BusTrip, bool>[trips.size()];

        multiset<BusTrip>::iterator temp_it;

        int u = 0;
        for (temp_it = trips.begin(); temp_it != trips.end(); ++temp_it)
        {
            if (u == trips.size())
                break;
            (temp + u)->first = (*temp_it);
            (temp + u)->second = false;
            u++;
        }

        

        while (res != trips.size())
        {
            isTreated = false;

            vector<string>* cluster = new vector<string>;
            
            ptr1 = temp;

            while ((*ptr1).second != false)
                ptr1++;

            // add from depot duration

            int duree_depot_fs = TargetInterTrip::findDurationByTargetId((*ptr1).first.busStationDep->getId(), stationsTargets[depotId]);
            if (duree_depot_fs == -1)
            {
                cout << "duration between depot and " << (*ptr1).first.busStationDep->getId() << " not found" << endl;
                exit(-1);
            }
            else {
                 if(mode == 0) cluster->push_back("Depot(" + to_string(duree_depot_fs) + ")");
            }

            if (ptr1 == (temp + trips.size() - 1))
            {
                int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                if (duree == -1)
                {
                    cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                    exit(-1);
                }
                (*ptr1).second = true;
                res++; 
                cluster->push_back((*ptr1).first.tripId);
                if (mode == 0) cluster->push_back("Depot(" + to_string(duree) + ")");
                allClusters.push_back(cluster);
                isTreated = true;
            }
            else
            {
                ptr2 = ptr1;
                while (!isTreated)
                {

                    while ((*ptr2).second && ptr2 != (temp + trips.size() - 1))
                    {
                        ptr2++;
                    }
                    if (ptr2 == (temp + trips.size() - 1))
                    {
                        int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                        if (duree == -1)
                        {
                            cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                            exit(-1);
                        }
                        (*ptr1).second = true;
                        res++;
                        cluster->push_back((*ptr1).first.tripId);
                        if (mode == 0) cluster->push_back("Depot(" + to_string(duree) + ")");
                        allClusters.push_back(cluster);
                        isTreated = true;
                    }
                    else
                    {
                        if ((*ptr2).first.busStationDep == (*ptr1).first.busStationArr)
                        {
                            if (0 <= difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 &&
                                difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 <= MIN_WAIT)
                            {
                                cluster->push_back((*ptr1).first.tripId);
                                (mode == 0) ? cluster->push_back("WS(" + to_string((int)difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60) + ")") : cluster->push_back("WS");
                                (*ptr1).second = true;
                                res++;
                                ptr1 = ptr2;
                                ptr2++;
                            }
                            // new cluster
                            else if (difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 > MIN_WAIT)
                            {
                                int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                                if (duree == -1)
                                {
                                    cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                                    exit(-1);
                                }
                                if (mode == 0) cluster->push_back("Depot(" + to_string(duree) + ")");
                                allClusters.push_back(cluster);
                                (*ptr1).second = true;
                                res++;
                                isTreated = true;
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
                                    if (attente > MIN_WAIT)
                                    {
                                        cluster->push_back((*ptr1).first.tripId);
                                        int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                                        if (duree == -1)
                                        {
                                            cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                                            exit(-1);
                                        }
                                        if (mode == 0) cluster->push_back("Depot(" + to_string(duree) + ")");
                                        allClusters.push_back(cluster);
                                        (*ptr1).second = true;
                                        res++;
                                        isTreated = true;
                                    }
                                    else
                                    {
                                        cluster->push_back((*ptr1).first.tripId);
                                        (mode == 0) ? cluster->push_back("HLP(" + to_string(duree) + "," + to_string(attente) + ")") : cluster->push_back("HLP");
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
        }
        return allClusters;
}

void write_cluster_to_file(ofstream& output_file, vector<vector<string>*> allClusters) {
    for (int i = 0; i < allClusters.size(); i++)
    {
        output_file << "cluster " << i << ": ";
        for (int j = 0; j < allClusters[i]->size(); j++)
        {
            output_file << (*allClusters[i])[j] << " ";
        }
        output_file << ";" << endl;
    }
}