#include "GraphGenerator.h"
#include "BusStationsHandler.h"
#include <list>
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

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips)
{
    
    TRIPS_MAP::iterator global_iterator;
    int iii = 0;
    // handle each Line
    string depotId = findDepotId(busStations);
    if (depotId == "") {
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

        int dest_fal = 0;
        iii++;
        // show Line
        cout << "Line: " << iii << " (" << (*global_iterator).first << " ) / " << linesTrips.size() << endl
             << "--------------" << endl;

        while (res != (*global_iterator).second->size())
        {
            int duree_total = 0;
            int duree_attente = 0;
            int duree_hlp = 0;
            int hlp_number = 0;
            double cout_total = 0;
            isTreated = false;
            clusterCount++;

            cout << endl << "-----------------------" << endl; 
            cout << "Cluster: " << clusterCount << endl;
            cout << endl << "---------------------" << endl << endl; 
            cout << "Depot --> "; 
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
                int duree =  TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.busStationArr->getId()]);
                if (duree == -1)
                {
                    cout << "duration between " << (*ptr1).first.busStationArr->getId() << " and depot not found" << endl;
                    exit(-1);
                }
                duree_total += duree;
                cout_total += duree * c_v;
                (*ptr1).second = true;
                res++;
                cout << (*ptr1).first.tripId << "--> Depot" << endl;
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
                        cout << (*ptr1).first.tripId << "--> Depot" << endl;
                        isTreated = true;
                        duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                    }
                    else
                    {
                        if ((*ptr2).first.busStationDep == (*ptr1).first.busStationArr)
                        {
                            if (0 <= difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 &&
                                difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 <= 45)
                            {
                                cout << (*ptr1).first.tripId << "--> waitInStation(" << difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 << ") --->";
                                (*ptr1).second = true;
                                res++;

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
                                cout << (*ptr1).first.tripId << " ---> Depot" << endl;
                                (*ptr1).second = true;
                                // added by omar <=>
                                    // we need to add trip duration even if he had to go to depot
                                duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                                // <=>
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
                                    if (attente > 45)
                                    {
                                        cout << (*ptr1).first.tripId << " --> Depot" << endl;
                                        // added by omar <=>
                                            // we need to add trip duration even if he had to go to depot
                                        duree_total += (difftime((*ptr1).first.dateArr, (*ptr1).first.dateDep) / 60);
                                        // <=>
                                        (*ptr1).second = true;
                                        res++;
                                        isTreated = true;
                                    }
                                    else
                                    {
                                        duree_attente += attente;
                                        duree_hlp += duree;
                                        duree_total += attente + duree;
                                        hlp_number++;
                                        cout << (*ptr1).first.tripId << " -- HLP --> "
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
            
            cout << " __________________________________________________________________________________ " << endl;
            cout << "|#Cluster|Duree Total|Cout total|Nombre HLP|Duree HLP|% HLP|Duree Attente|% Attente|" << endl;
            
            cout_total += c_a * duree_attente + c_v * duree_hlp;
            cout << "|" << setw(8) << clusterCount << "|" << setw(7) << duree_total << " min|" << setw(10) << cout_total << "|" << setw(6) << hlp_number << " HLP|" << setw(5) << duree_hlp << " min|" << setw(3) << duree_hlp*100/duree_total << " %|" << setw(9) << duree_attente << " min|" << setw(7) << duree_attente * 100 / duree_total << " %|" << endl;
            cout << " ---------------------------------------------------------------------------------- " << endl;
            duree_total = 0;
            duree_attente = 0;
            duree_hlp = 0;
        }
    }

    cout << "-------------" << endl;
    cout << "Finished all Line Trips" << endl;
    cout << "-------------" << endl;
}