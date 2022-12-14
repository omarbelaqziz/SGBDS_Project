#include <iostream>

#include <stdio.h>
#include "../SGBDS_Project/preproc.h"
#include "ilcplex/ilocplex.h"

using namespace std;

#define NOT_EXIST_VALUE MAXFLOAT

typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;

string get_trip_id_from_trips_population(int index, multiset<BusTrip> &tripsPopulation);
float existCostFromDepotAndCalculate(string depotId, string target, INTER_TRIPS interTrips);
float existCostFromTripToDepotAndCalculate(string source, string depotId, INTER_TRIPS interTrips);
float existCostBetweenTwotTripsAndCalculate(BusTrip startTrip, BusTrip currentTrip, INTER_TRIPS interTrips);
string get_state_between_two_trips(string trip1, string trip2, multiset<BusTrip> &tripsPopulation); 

int main(int argc, char const *argv[])
{

#pragma region parsing data from dataset
    // collecting data
    string data_file = argv[1];
    string output_file = argv[2];

    set<BusStation> *busStations = nullptr;
    INTER_TRIPS interTrips;
    TRIPS_MAP tripsStations;

    ifstream dataFile;
    dataFile.open(data_file);
    if (!dataFile.is_open())
    {
        cerr << "Couldn't move forward " << endl;
        exit(-1);
    }
    string line;
    while (getline(dataFile, line))
    {
        if (line.find("{") != string::npos)
        {
            if (line.find("BusStations") != string::npos)
            {
                busStations = handle_file_stream_bus_stations(dataFile);
            }
            else if (line.find("BusTrip") != string::npos)
            {
                handle_file_stream_bus_trips(tripsStations, dataFile, busStations);
            }
            else if (line.find("InterTrips") != string::npos)
            {
                handle_file_stream_inter_trips(interTrips, dataFile, busStations);
            }
        }
        else if (line.find("}") != string::npos)
        {
            // std::cout << "Op. ends at " << line << endl;
        }
    }

    string depotId = findDepotId(busStations);

    multiset<BusTrip> busTripsPopulation;
    detach_lines(busTripsPopulation, tripsStations);
#pragma endregion
    cout << "Parsing data from dataset : DONE" << endl;

#pragma region Problem Data

    float **C;
    int N = busTripsPopulation.size();
    int M = N * (1.0 / 13.0);

    C = (float **)malloc(sizeof(float *) * (N + 2));

    for (int i = 0; i < N + 2; i++)
    {
        C[i] = (float *)malloc(sizeof(float) * (N + 2));
    }

    int i = 0;
    int j = 0;
    int k = 0;

    cout << "Size : " << N << endl;

    multiset<BusTrip>::iterator it_to_i_trip;
    multiset<BusTrip>::iterator it_to_j_trip;

    C[0][0] = NOT_EXIST_VALUE;
    C[N + 1][N + 1] = NOT_EXIST_VALUE;
    C[0][N + 1] = NOT_EXIST_VALUE;
    C[N + 1][0] = NOT_EXIST_VALUE;


    for (i = 1, it_to_i_trip = busTripsPopulation.begin(); it_to_i_trip != busTripsPopulation.end(); ++i, ++it_to_i_trip)
    {

        C[i][0] = existCostFromTripToDepotAndCalculate((*it_to_i_trip).getBusStationArr()->getId(), depotId, interTrips); 
        C[i][N + 1] = existCostFromTripToDepotAndCalculate((*it_to_i_trip).getBusStationArr()->getId(), depotId, interTrips);
        C[0][i] = existCostFromDepotAndCalculate(depotId, (*it_to_i_trip).getBusStationDep()->getId(), interTrips);
        C[N + 1][i] = existCostFromDepotAndCalculate(depotId, (*it_to_i_trip).getBusStationDep()->getId(), interTrips);
        

        for (j = 1, it_to_j_trip = busTripsPopulation.begin(); it_to_j_trip != busTripsPopulation.end(); ++it_to_j_trip, ++j)
        {
            if (i != j)
            {
                C[i][j] = existCostBetweenTwotTripsAndCalculate((*it_to_i_trip), (*it_to_j_trip), interTrips);
                
            }
            else
            {
                C[i][j] = NOT_EXIST_VALUE;
            }
        }
    }

    cout << endl
         << "done" << endl;
#pragma endregion
    cout << "Calculating problem data : DONE" << endl;

#pragma region Set up environment
    IloEnv env;
    IloModel Model(env);
#pragma endregion
    cout << "Set up environment : DONE" << endl;

#pragma region Define decision variables
    NumVar3D X(env, N + 2);

    for (i = 0; i < N + 2; i++)
    {
        X[i] = NumVar2D(env, N + 2);
        for (j = 0; j < N + 2; j++)
        {
            X[i][j] = IloNumVarArray(env, N + 2);
            for (k = 0; k < M; k++)
            {
                X[i][j][k] = IloNumVar(env, IloFalse, IloTrue, ILOINT);
            }
        }
    }

#pragma endregion
    cout << "Define decicion variables : DONE" << endl;

#pragma region Define Objective Function
    // objective function
    IloExpr exp0(env);

    // N : la somme total des trips
    // M : le nombre total des vehicule

    for (k = 0; k < M; k++)
    {
        for (i = 0; i < N + 2; i++)
        {
            for (j = 0; j < N + 2; j++)
            {
                exp0 += (C[i][j] * X[i][j][k]);
            }
        }
    }

    Model.add(IloMinimize(env, exp0));

#pragma endregion
    cout << "Define objective function : DONE" << endl;

#pragma region Constraints

    // 3.2
    for (i = 1; i < N + 1; i++)
    {
        IloExpr exp2(env);
        for (k = 0; k < M; k++)
        {
            for (j = 0; j < N + 2; j++)
            {
                exp2 += X[i][j][k];
            }
        }
        Model.add(exp2 == 1);
    }

    // 3.4
    for (k = 0; k < M; k++)
    {
        IloExpr exp3(env);
        for (j = 0; j < N + 2; j++)
        {
            exp3 += X[0][j][k];
        }
        Model.add(exp3 == 1);
    }

    // 3.5
    int h = 0;
    for (h = 1; h < N + 1; h++)
    {
        for (k = 0; k < M; k++)
        {
            IloExpr exp4(env);
            IloExpr exp5(env);
            for (i = 0; i < N + 2; i++)
            {
                exp4 += X[i][h][k];
                exp5 += X[h][i][k];
            }
            Model.add((exp4 - exp5) == 0);
        }
    }

    // 3.6
    for (k = 0; k < M; k++)
    {
        IloExpr exp6(env);
        for (i = 0; i < N + 2; i++)
        {
            exp6 += X[i][N + 1][k];
        }
        Model.add(exp6 == 1);
    }

#pragma endregion

    IloCplex cplex(Model);
    cplex.setOut(env.getNullStream());

    if (!cplex.solve())
    {
        env.error() << "Failed to optimize the Master Problem!!!" << endl;
        throw(-1);
    }

    double obj = cplex.getObjValue();

    cout << "cost: " << obj << endl;

    // serching the first start
    for (k = 0; k < M; k++)
    {
        int start = 0;
        for (j = 0; j < N + 2; j++)
        {
            int tt = cplex.getValue(X[0][j][k]);
            if (tt == 1)
                start = j;
        }

        string startTripId = get_trip_id_from_trips_population(start-1, busTripsPopulation); 
        

        cout << "cluster " << k+1 << " : " << startTripId << " ";
        while (start != N + 1)
        {
            // search the next trip
            int next = 0;
            for (j = 0; j < N + 2; j++)
            {
                int tt = cplex.getValue(X[start][j][k]);
                if (tt == 1)
                    next = j;
            }
            string arrivalTripId = get_trip_id_from_trips_population(next-1, busTripsPopulation); 
            (next < N + 1) ? cout << get_state_between_two_trips(startTripId, arrivalTripId, busTripsPopulation) << " " << arrivalTripId << " " : cout << "";
            
            start = next;
            startTripId = arrivalTripId; 
        }
        cout << ";" << endl;
    } 
    cout << endl
         << "=----------------=" << endl;

    cout << endl;

    return 0;
}

float existCostFromDepotAndCalculate(string depotId, string target, INTER_TRIPS interTrips)
{
    int duration = TargetInterTrip::findDurationByTargetId(target, interTrips.at(depotId));
    if (duration == -1)
    {
        return NOT_EXIST_VALUE;
    }
    return duration * c_v;
}

float existCostFromTripToDepotAndCalculate(string source, string depotId, INTER_TRIPS interTrips)
{
    int duration = TargetInterTrip::findDurationByTargetId(depotId, interTrips.at(source));
    if (duration == -1)
    {
        return NOT_EXIST_VALUE;
    }
    return duration * c_v;
}

float existCostBetweenTwotTripsAndCalculate(BusTrip startTrip, BusTrip currentTrip, INTER_TRIPS interTrips)
{
    // if we have the same arr and dep stations
    if (startTrip.getBusStationArr()->getId() == currentTrip.getBusStationDep()->getId())
    {
        int diff_time = difftime(currentTrip.getDateDep(), startTrip.getDateArr()) / 60;
        // SI ON PEUT TOLERER ATTENTE = 0 DANS UNE STATION
        if (0 <= diff_time && diff_time <= MIN_WAIT) // the successor could be the next
        {
            return diff_time * c_a;
        }
    }
    // when  (HLP)
    else
    {
        int temp_dur = TargetInterTrip::findDurationByTargetId(currentTrip.getBusStationDep()->getId(), interTrips.at(startTrip.getBusStationArr()->getId()));
        if (temp_dur != -1)
        {
            int attente_hlp = (difftime(currentTrip.getDateDep(), startTrip.getDateArr()) / 60.0) - temp_dur;

            if (0 <= attente_hlp && attente_hlp <= MIN_WAIT)
            {
                // verifier si HLP
                return temp_dur * c_v + attente_hlp * c_a;
            }
        }
    }
    return NOT_EXIST_VALUE;
}

// This function will take the multiset and an index and gonna return the Trip id
string get_trip_id_from_trips_population(int index, multiset<BusTrip> &tripsPopulation)
{

    if (0 <= index && index < tripsPopulation.size())
    {
        int temp_index = 0;
        multiset<BusTrip>::iterator it;
        for (it = tripsPopulation.begin(); it != tripsPopulation.end(); ++it)
        {
            if (temp_index == index)
                return it->getTripId();

            temp_index++;
        }
    }
    return "NULL";
}

string get_state_between_two_trips(string trip1, string trip2, multiset<BusTrip> &tripsPopulation)
{
    BusTrip start, target;
    if (findTripById(start, trip1, tripsPopulation) && findTripById(target, trip2, tripsPopulation))
    {
        if (start.getBusStationArr()->getId() == target.getBusStationDep()->getId())
            return "WS";

        else
            return "HLP";
    }

    cerr << endl << RED << "[ Fatal error ] " << RESET << YELLOW << "No trip found with folowing data " << RESET << trip1 << " && " << trip2 << endl;
    exit(-1);
}
