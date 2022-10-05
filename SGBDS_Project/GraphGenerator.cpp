#include "GraphGenerator.h"

void graph_generator(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    TRIPS_MAP linesTrips)
{
    TRIPS_MAP::iterator global_iterator;

    std::allocator<pair<BusTrip, bool> > alloc; 

    // handle each Line
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

        temp = alloc.allocate(sizeof(pair<BusTrip, bool>) * (*global_iterator).second->size());

        multiset<BusTrip>::iterator temp_it;

        int u = 0;
        for (temp_it = (*global_iterator).second->begin(); temp_it != (*global_iterator).second->end(); ++temp_it)
        {
            if (u == (*global_iterator).second->size())
                break;
            temp[u].first = (*temp_it);
            temp[u].second = false;
            u++;
        }

        int dest_fal = 0; 

        // show Line
        cout << "Line: " << (*global_iterator).first << endl
             << "--------------" << endl;

        while (res != (*global_iterator).second->size())
        {
            isTreated = false;
            clusterCount++;

            cout << "Cluster: " << clusterCount << endl;

            ptr1 = temp;

            while ((*ptr1).second != false)
                ptr1++;


            ptr2 = ptr1;
            ptr2++;

            while (!isTreated)
            {
                while ((*ptr2).second && ptr2 != (temp + (*global_iterator).second->size() - 1))
                {
                    ptr2++;
                }
                if (ptr2 == (temp + (*global_iterator).second->size() - 1))
                {
                    (*ptr2).second = true;
                    res++;
                    cout << (*ptr1).first.tripId << "==> Depot" << endl;
                    isTreated = true;
                }
                else
                {
                    if ((*ptr2).first.busStationDep == (*ptr1).first.busStationArr)
                    {
                        if (0 <= difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 &&
                            difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 <= 45)
                        {
                            cout << (*ptr1).first.tripId << "==> waitInStation(" << difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60<< ") ===>";
                            (*ptr1).second = true;
                            res++;
                            ptr1 = ptr2;
                            ptr2++;
                        }
                        if (difftime((*ptr2).first.dateDep, (*ptr1).first.dateArr) / 60 > 45)
                        {
                            cout << (*ptr1).first.tripId << " ===> Depot" << endl;
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
                        auto itty = stationsTargets[(*ptr1).first.busStationArr->id]->find(TargetInterTrip((*ptr2).first.busStationDep, "asdfsa", 12));
                        if (itty == stationsTargets[(*ptr1).first.busStationDep->id]->end())
                        {
                            cout << "Error in finding the destination ";
                            dest_fal++; 
                        }
                        int attente = difftime((*ptr1).first.dateArr, (*ptr2).first.dateDep) / 60 - (*itty).duree;
                        if (attente > 0)
                        {
                            if (attente > 45)
                            {
                                cout << (*ptr1).first.tripId << " ===> Depot" << endl;
                                (*ptr1).second = true;
                                res++;
                                isTreated = true;
                            }
                            else
                            {
                                cout << (*ptr1).first.tripId << " === HLP ===> "
                                     << " waitInStation (" << attente << ")"
                                     << " ===>";
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