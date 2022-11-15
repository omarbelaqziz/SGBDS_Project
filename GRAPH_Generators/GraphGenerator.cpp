#include "GraphGenerator.h"
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

            duree_total += TargetInterTrip::findDurationByTargetId((*ptr1).first.getBusStationDep()->getId(), stationsTargets[depotId]);
            if (duree_total == -1)
            {
                cout << "duration between depot and " << (*ptr1).first.getBusStationDep()->getId() << " not found" << endl;
                exit(-1);
            }
            cout_total += duree_total * c_v;

            if (ptr1 == (temp + (*global_iterator).second->size() - 1))
            {
                int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);
                if (duree == -1)
                {
                    cout << "duration between " << (*ptr1).first.getBusStationArr()->getId() << " and depot not found" << endl;
                    exit(-1);
                }
                duree_total += duree;
                cout_total += duree * c_v;
                (*ptr1).second = true;
                res++;
                depot_stats.somme_covered_voyages++;
                output_file << (*ptr1).first.getTripId() << "--> Depot" << endl;
                isTreated = true;
                duree_total += (difftime((*ptr1).first.getDateArr(), (*ptr1).first.getDateDep()) / 60);
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
                        int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);
                        if (duree == -1)
                        {
                            cout << "duration between " << (*ptr1).first.getBusStationArr()->getId() << " and depot not found" << endl;
                            exit(-1);
                        }
                        duree_total += duree;
                        cout_total += duree * c_v;
                        (*ptr1).second = true;
                        res++;
                        output_file << (*ptr1).first.getTripId() << "--> Depot" << endl;
                        isTreated = true;
                        depot_stats.somme_covered_voyages++;
                        duree_total += (difftime((*ptr1).first.getDateArr(), (*ptr1).first.getDateDep()) / 60);
                    }
                    else
                    {
                        if ((*ptr2).first.getBusStationDep() == (*ptr1).first.getBusStationArr())
                        {
                            if (0 <= difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60 &&
                                difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60 <= MIN_WAIT)
                            {
                                output_file << (*ptr1).first.getTripId() << "--> waitInStation(" << difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60 << ") --->";
                                (*ptr1).second = true;
                                res++;

                                duree_attente += difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60;
                                duree_total += (difftime((*ptr1).first.getDateArr(), (*ptr1).first.getDateDep()) / 60 + difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60);
                                ptr1 = ptr2;
                                ptr2++;
                            }
                            // new cluster
                            else if (difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60 > MIN_WAIT)
                            {
                                int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);
                                if (duree == -1)
                                {
                                    cout << "duration between " << (*ptr1).first.getBusStationArr()->getId() << " and depot not found" << endl;
                                    exit(-1);
                                }
                                duree_total += duree;
                                cout_total += duree * c_v;
                                output_file << (*ptr1).first.getTripId() << " ---> Depot" << endl;
                                (*ptr1).second = true;
                                duree_total += (difftime((*ptr1).first.getDateArr(), (*ptr1).first.getDateDep()) / 60);
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
                            auto PTRR = (*ptr2).first.getBusStationDep();

                            if (PTRR == NULL)
                            {
                                cout << endl
                                     << "---> There is no no data for ptr2 :: " << endl;
                                exit(-1);
                            }
                            else
                            {
                                int duree = TargetInterTrip::findDurationByTargetId((*ptr2).first.getBusStationDep()->getId(), stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);

                                if (duree == -1)
                                {

                                    // SEARCH IN THE NEGATIVE WAY
                                    int temp_duree = TargetInterTrip::findDurationByTargetId((*ptr1).first.getBusStationArr()->getId(), stationsTargets[(*ptr2).first.getBusStationDep()->getId()]);
                                    if (temp_duree == -1)
                                    {
                                        cout << endl
                                             << "---> There is no InterTrip between " << (*ptr1).first.getBusStationArr()->getId() << " -> " << (*ptr2).first.getBusStationDep()->getId() << " <-- " << endl;
                                        exit(-1);
                                    }
                                    else
                                    {
                                        duree = temp_duree;
                                    }
                                }
                                int attente = (difftime((*ptr2).first.getDateDep(), (*ptr1).first.getDateArr()) / 60) - duree;

                                if (attente > 0)
                                {
                                    if (attente > MIN_WAIT)
                                    {
                                        // added by omar <=>
                                        // we need to add trip duration even if he had to go to depot
                                        duree_total += (difftime((*ptr1).first.getDateArr(), (*ptr1).first.getDateDep()) / 60);
                                        // <=>
                                        output_file << (*ptr1).first.getTripId() << " --> Depot" << endl;
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
                                        output_file << (*ptr1).first.getTripId() << " -- HLP --> "
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

    cout << "|" << setw(8) << depotId << "|" << setw(7) << depot_stats.duree_total_depot << " min|" << setw(10) << depot_stats.cout_total_depot << "|" << setw(6) << depot_stats.duree_total_hlp << " HLP|" << setw(5) << depot_stats.somme_hlp << " min|" << setw(3) << depot_stats.duree_total_hlp * 100 / depot_stats.duree_total_depot << " %|" << setw(9) << depot_stats.duree_total_attente << " min|" << setw(7) << depot_stats.duree_total_attente * 100 / depot_stats.duree_total_depot << " %|" << depot_stats.somme_clusters << "|" << setw(7) << depot_stats.somme_covered_voyages << endl;
    cout << " ---------------------------------------------------------------------------------- " << endl;
}

vector<vector<string>> clusters_generator_fromTripsSet(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    multiset<BusTrip> trips)
{
    vector<vector<string>> allClusters;

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
    pair<BusTrip, bool> *ptr3;

    temp = new pair<BusTrip, bool>[trips.size()];

    
    

    // fill temp with data
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

    // while we didnt cover all trips
    while (res <= trips.size())
    {
        vector<string> cluster;
        isTreated = false;
        ptr1 = temp;

        // find first not visited trip
        while ((*ptr1).second != false && ptr1 != (temp + trips.size() -1))
            ptr1++;

        

        // add from duration from depot to first station
        int duree_depot_fs = TargetInterTrip::findDurationByTargetId((*ptr1).first.getBusStationDep()->getId(), stationsTargets[depotId]);
        if (duree_depot_fs == -1)
        {
            cout << "duration between depot and " << (*ptr1).first.getBusStationDep()->getId() << " not found" << endl;
            exit(-1);
        }

        if (ptr1 == (temp + trips.size() - 1))
        {
            if(ptr1->second == false){
                int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);
                if (duree == -1)
                {
                    cout << "duration between " << (*ptr1).first.getBusStationArr()->getId() << " and depot not found" << endl;
                    exit(-1);
                }
                (*ptr1).second = true;
                res++;
                cluster.push_back((*ptr1).first.getTripId());
                allClusters.push_back(cluster);
                isTreated = true;
            }
            break;
        }
        else
        {
            ptr2 = ptr1;
            while (!isTreated)
            {
                while ((*ptr2).second != false && ptr2 != (temp + trips.size() - 1))
                    ptr2++;
                
                if (ptr2 == (temp + trips.size() - 1))
                {
                    int duree = TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[(*ptr1).first.getBusStationArr()->getId()]);
                    if (duree == -1)
                    {
                        cout << "duration between " << (*ptr1).first.getBusStationArr()->getId() << " and depot not found" << endl;
                        exit(-1);
                    }
                    (*ptr1).second = true;
                    res++;
                    cluster.push_back((*ptr1).first.getTripId());
                    float cost;
                    string type;
                    if(ptr2->second == false && twoTripsCost(cost,stationsTargets,busStations,(*ptr1).first,(*ptr2).first,type)){
                        (*ptr2).second = true;
                        res++;
                        cluster.push_back(type);
                        cluster.push_back((*ptr2).first.getTripId());
                    }
                    allClusters.push_back(cluster);
                    isTreated = true;
                }
                else
                {
                    float cost,secondcost;
                    string type,secondtype;
                    if(twoTripsCost(cost,stationsTargets,busStations,(*ptr1).first,(*ptr2).first,type)){
                        ptr3 = ptr2;
                        while ((*ptr3).second && ptr3 != (temp + trips.size() - 1))
                        {
                            if(twoTripsCost(secondcost,stationsTargets,busStations,(*ptr1).first,(*ptr3).first,secondtype)){
                                if(secondcost < cost){
                                    cost = secondcost;
                                    ptr2 = ptr3;
                                    type = secondtype;
                                }
                            }
                        }
                        (*ptr1).second = true;
                        res++;
                        cluster.push_back((*ptr1).first.getTripId());
                        cluster.push_back(type);
                        ptr1 = ptr2;
                        ptr2++;
                    }
                    else{
                        ptr2++;
                    }
                }
            }
        }
    }
    return allClusters;
}

vector<vector<string>> optimize_generated_solution(
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    multiset<BusTrip> trips,
    vector<vector<string>> solution) {
    for (int i = 0; i < solution.size(); i++)
    {
        // cout << "enter i " << i << endl;
        int size = solution.at(i).size();
        float cost;
        for (int j = i + 1; j < solution.size(); j++)
        {
            // cout << "enter j" << j << endl;
            BusTrip depart, arrivee;
            if(findTripById(depart,solution.at(i).at(size - 1),trips) 
            &&
            findTripById(arrivee,solution.at(i).at(0),trips)){
                string type;
                if(twoTripsCost(cost,stationsTargets,busStations,depart,arrivee,type)){
                    solution.at(i).insert(solution.at(i).end(),solution.at(j).begin(),solution.at(j).end());
                    solution.erase(solution.begin()+j);
                    break;
                }
            }
        }
    }
    return solution;
}

void write_cluster_to_file(ofstream &output_file, vector<vector<string>> allClusters)
{
    for (int i = 0; i < allClusters.size(); i++)
    {
        output_file << "cluster " << i + 1 << " : ";
        for (int j = 0; j < allClusters[i].size(); j++)
        {
            output_file << allClusters[i][j] << " ";
        }
        output_file << ";" << endl;
    }
}

void stats_calculator(
    vector<vector<double>> &clustersStats,
    vector<double> &depotStats,
    vector<vector<string>> clusters,
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    multiset<BusTrip> trips)
{
    // total stats of depot params
    int totalWaitDuration = 0;
    int totalHlpDuration = 0;
    int totalHlpNumber = 0;
    int totalTripsDuration = 0;
    double totalCost = 0.0;
    int totalCoveredTrips = 0;

    // iterate over all clusters
    for (int i = 0; i < clusters.size(); i++)
    {
        // cout << "start cluster " << i + 1 << endl;
        // stats params
        int waitDuration = 0;
        int hlpDuration = 0;
        int fromAndToDepotDuration = 0;
        int hlpNumber = 0;
        int tripsDuration = 0;
        int coveredTripsInCluster = 0;

        // load depot ID
        string depotId = findDepotId(busStations);

        BusTrip firstTrip, lastTrip;

        // add duration between depot and first station
        bool foundFirst = findTripById(firstTrip, clusters[i][0], trips);
        if (!foundFirst)
        {
            cout << "didnt found trip " << clusters[i][0] << endl;
            exit(-1);
        }
        fromAndToDepotDuration += TargetInterTrip::findDurationByTargetId(firstTrip.getBusStationDep()->getId(), stationsTargets[depotId]);

        // iterate over the cluster
        for (int j = 0; j < clusters[i].size(); j++)
        {
            // case of wait in station
            if (clusters[i][j] == "WS")
            {
                BusTrip startTrip, nextTrip;
                bool foundStart = findTripById(startTrip, clusters[i][j - 1], trips);
                bool foundNext = findTripById(nextTrip, clusters[i][j + 1], trips);
                if (!foundStart)
                {
                    cout << "didnt found trip " << clusters[i][j - 1] << endl;
                    exit(-1);
                }
                else if (!foundNext)
                {
                    cout << "didnt found trip " << clusters[i][j + 1] << endl;
                    exit(-1);
                }
                else
                {
                    waitDuration += (difftime(nextTrip.getDateDep(), startTrip.getDateArr()) / 60);
                }
            }

            // case of HLP
            else if (clusters[i][j] == "HLP")
            {
                BusTrip startTrip, nextTrip;
                bool foundStart = findTripById(startTrip, clusters[i][j - 1], trips);
                bool foundNext = findTripById(nextTrip, clusters[i][j + 1], trips);
                if (!foundStart)
                {
                    cout << "didnt found trip " << clusters[i][j - 1] << endl;
                    exit(-1);
                }
                else if (!foundNext)
                {
                    cout << "didnt found trip " << clusters[i][j + 1] << endl;
                    exit(-1);
                }
                else
                {
                    hlpNumber++;
                    int temp_duration = TargetInterTrip::findDurationByTargetId(nextTrip.getBusStationDep()->getId(), stationsTargets.at(startTrip.getBusStationArr()->getId()));
                    if (temp_duration == -1)
                    {
                        cout << "error retrieving intertrip duratuion from : " << startTrip.getBusStationArr()->getId() << " to " << nextTrip.getBusStationDep()->getId() << endl;
                        exit(-1);
                    }
                    else
                    {
                        hlpDuration += temp_duration;
                        waitDuration += (difftime(nextTrip.getDateDep(), startTrip.getDateArr()) / 60) - temp_duration;
                    }
                }
            }
            // case of a trip
            else
            {
                if (j == clusters[i].size() - 1)
                {
                    BusTrip busTrip;
                    bool foundTrip = findTripById(busTrip, clusters[i][j], trips);
                    if (!foundTrip)
                    {
                        cout << "didnt found trip " << clusters[i][j] << endl;
                        exit(-1);
                    }
                    fromAndToDepotDuration += TargetInterTrip::findDurationByTargetId(depotId, stationsTargets[busTrip.getBusStationArr()->getId()]);
                }
                BusTrip busTrip;
                bool foundTrip = findTripById(busTrip, clusters[i][j], trips);
                if (!foundTrip)
                {
                    cout << "didnt found trip " << clusters[i][j] << endl;
                    exit(-1);
                }
                coveredTripsInCluster++;
                tripsDuration += (difftime(busTrip.getDateArr(), busTrip.getDateDep()) / 60);
            }
        }

        // load statistics into clusters stats:
        vector<double> singleClusterStats;

        // add duration to total params
        totalHlpDuration += hlpDuration;
        totalWaitDuration += waitDuration;
        totalTripsDuration += tripsDuration + fromAndToDepotDuration;

        // index 0 : total duaration of cluster = trips duration + wait duration + hlp duration
        double totalClusterDuration = (double)tripsDuration + (double)waitDuration + (double)hlpDuration;
        singleClusterStats.push_back(totalClusterDuration);

        // index 1 : total cost of cluster = (wait duration * c_a) + (hlp duration * c_v)
        double costCluster = (waitDuration * c_a) + (hlpDuration * c_v) + (fromAndToDepotDuration * c_v);
        singleClusterStats.push_back(costCluster);
        totalCost += costCluster;

        // index 2 :  number of HLPs in cluster
        singleClusterStats.push_back(hlpNumber);
        totalHlpNumber += hlpNumber;

        // index 3 : hlp duration
        singleClusterStats.push_back(hlpDuration);

        // index 4 : hlp pourcentage = hlp duration / total duration
        singleClusterStats.push_back(hlpDuration / totalClusterDuration);

        // index 5 : wait duration
        singleClusterStats.push_back(waitDuration);

        // index 6 : wait pourcentage
        singleClusterStats.push_back(waitDuration / totalClusterDuration);

        // index 7 : number of covered trips
        singleClusterStats.push_back(coveredTripsInCluster);
        totalCoveredTrips += coveredTripsInCluster;

        // add the vector to clustersStats
        clustersStats.push_back(singleClusterStats);
    }
    // add global depot stats
    // ! same indexes as previous and index 8 is the number of clusters
    double totalDepotDuration = (double)totalHlpNumber + (double)totalWaitDuration + (double)totalTripsDuration;
    depotStats.push_back(totalDepotDuration);
    depotStats.push_back((double)totalCost + (clusters.size() * FIX_COST));
    depotStats.push_back(totalHlpNumber);
    depotStats.push_back(totalHlpDuration);
    depotStats.push_back(totalHlpDuration / totalDepotDuration); // hlp pourcentage
    depotStats.push_back(totalWaitDuration);
    depotStats.push_back(totalWaitDuration / totalDepotDuration); // wait pourcentage
    depotStats.push_back(totalCoveredTrips);
    depotStats.push_back(clusters.size());
}

void writeStatsIntoScreen(
    const vector<vector<double>> &clustersStats,
    const vector<double> &depotStats, 
    ofstream &stats_out_stream)
{
    stats_out_stream << "Total cluster : " << clustersStats.size() << endl; 
    stats_out_stream << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
    stats_out_stream << "| Cluster | Duration | Cluster's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips |" << endl;
    stats_out_stream << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
    for (int i = 0; i < clustersStats.size(); i++)
    {
        stats_out_stream << "|" << setw(9) << i;
        stats_out_stream << "|" << setw(10) << clustersStats[i][0];
        stats_out_stream << "|" << setw(16) << clustersStats[i][1];
        stats_out_stream << "|" << setw(16) << clustersStats[i][2];
        stats_out_stream << "|" << setw(14) << clustersStats[i][3];
        stats_out_stream << "|" << setw(17) << clustersStats[i][4];
        stats_out_stream << "|" << setw(15) << clustersStats[i][5];
        stats_out_stream << "|" << setw(18) << clustersStats[i][6];
        stats_out_stream << "|" << setw(17) << clustersStats[i][7] << "|" << endl;
    }
    stats_out_stream << "+---------+----------+----------------+----------------+--------------+-----------------+---------------+------------------+-----------------+" << endl;
    stats_out_stream << endl;

    cout << "DEPOT STATS : " << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    cout << "| Total Duration | Depot's Cost | Number of HLPs | HLP Duration | HLP pourcentage | Wait Duration | Wait Pourcentage | Number of trips | Number of Clusters |" << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
    cout << "|" << setw(16) << depotStats[0];
    cout << "|" << setw(14) << depotStats[1];
    cout << "|" << setw(16) << depotStats[2];
    cout << "|" << setw(14) << depotStats[3];
    cout << "|" << setw(17) << depotStats[4];
    cout << "|" << setw(15) << depotStats[5];
    cout << "|" << setw(18) << depotStats[6];
    cout << "|" << setw(17) << depotStats[7];
    cout << "|" << setw(20) << depotStats[8] << "|" << endl;
    cout << "+----------------+--------------+----------------+--------------+-----------------+---------------+------------------+-----------------+--------------------+" << endl;
}

bool twoTripsCost(float& cost, 
    INTER_TRIPS stationsTargets,
    set<BusStation> *busStations,
    BusTrip firstTrip,
    BusTrip secondTrip,
    string& type){
        if (secondTrip.getBusStationDep() == firstTrip.getBusStationArr())
        {
            if (0 <= difftime(secondTrip.getDateDep(), firstTrip.getDateArr()) / 60 &&
                difftime(secondTrip.getDateDep(), firstTrip.getDateArr()) / 60 <= MIN_WAIT)
            {
                cost = (difftime(secondTrip.getDateDep(), firstTrip.getDateArr()) / 60) * c_a;
                type = "WS";
                return true;
            }
            else {
                cost = -1.0;
                return false;
            }
            
        }
        else {
            int duree = TargetInterTrip::findDurationByTargetId(secondTrip.getBusStationDep()->getId(), stationsTargets[firstTrip.getBusStationArr()->getId()]);

            if (duree == -1)
            {
                // SEARCH IN THE NEGATIVE WAY
                int temp_duree = TargetInterTrip::findDurationByTargetId(firstTrip.getBusStationArr()->getId(), stationsTargets[secondTrip.getBusStationDep()->getId()]);
                if (temp_duree == -1)
                {
                    cout << endl
                            << "---> There is no InterTrip between " << firstTrip.getBusStationArr()->getId() << " -> " << secondTrip.getBusStationDep()->getId() << " <-- " << endl;
                    exit(-1);
                }
                else
                {
                    duree = temp_duree;
                }
            }
            int attente = (difftime(secondTrip.getDateDep(), firstTrip.getDateArr()) / 60) - duree;

            if (attente > 0 && attente <= MIN_WAIT)
            {
                cost = (attente * c_a) + (duree * c_v); 
                type = "HLP";
                return true;
            }
            else
            {
                cost = -1.0;
                return false;
            }
        }

}