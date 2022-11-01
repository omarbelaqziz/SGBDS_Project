#include <iostream>

#include <stdio.h>
#include "../SGBDS_Project/preproc.h"
#include "ilcplex/ilocplex.h"

using namespace std;

typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;

float existCostFromDepotAndCalculate(string depotId, BusTrip bt, INTER_TRIPS interTrips);
float existCostFromTripToDepotAndCalculate(string depotId, BusTrip bt, INTER_TRIPS interTrips);
float existCostBetweenTwotTripsAndCalculate(BusTrip startTrip, BusTrip currentTrip, INTER_TRIPS interTrips);

int main(int argc, char const *argv[])
{

#pragma region parsing data from dataset
    // collecting data
    string data_file = "../data/test60.txt";
    string output_file = "../output/out60.txt";

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
    int M = 9;

    C = (float **)malloc(sizeof(float *) * (N + 1));
    for (int i = 0; i < N + 1; i++)
    {
        C[i] = (float *)malloc(sizeof(float) * (N + 1));
    }

    int i = 0;
    int j = 0;
    int k = 0;

    cout << "Size : " << N << endl;

    multiset<BusTrip>::iterator it_to_i_trip;
    multiset<BusTrip>::iterator it_to_j_trip;
    C[0][0] = MAXFLOAT;

    for (i = 1, it_to_i_trip = busTripsPopulation.begin(); it_to_i_trip != busTripsPopulation.end(); i++, ++it_to_i_trip)
    {
        C[i][0] = existCostFromTripToDepotAndCalculate(depotId, *it_to_i_trip, interTrips);
        C[0][i] = existCostFromDepotAndCalculate(depotId, (*it_to_i_trip), interTrips);

        for (j = 1, it_to_j_trip = it_to_i_trip; it_to_j_trip != busTripsPopulation.end(); ++it_to_j_trip, j++)
        {
            if (i != j)
            {

                C[i][j] = existCostBetweenTwotTripsAndCalculate((*it_to_i_trip), (*it_to_j_trip), interTrips);

                // cout << cost << ", ";
            }
            else
            {
                C[i][j] = MAXFLOAT;
            }
        }
    }

    // for (i = 0; i <= N; i++)
    // {
    //     cout << C[i][i] << ", ";
    // }

#pragma endregion
    cout << "Calculating problem data : DONE" << endl;

#pragma region Set up environment
    IloEnv env;
    IloModel Model(env);
#pragma endregion
    cout << "Set up environment : DONE" << endl;

#pragma region Define decision variables
    NumVar3D X(env, N + 1);
    NumVar2D Y(env, N + 1);

    for (i = 1; i < N + 1; i++)
    {
        X[i] = NumVar2D(env, N + 1);
        for (j = 1; j < N + 1; j++)
        {
            X[i][j] = IloNumVarArray(env, N + 1);
            for (k = 0; k < M; k++)
            {
                X[i][j][k] = IloNumVar(env, IloFalse, IloTrue, ILOBOOL);
            }
        }
    }

    for (i = 0; i < N + 1; i++)
    {
        Y[i] = IloNumVarArray(env, N + 1);
        for (k = 0; k < M; k++)
        {
            Y[i][k] = IloNumVar(env, IloFalse, IloTrue, ILOBOOL);
        }
    }

#pragma endregion
    cout << "Define decicion variables : DONE" << endl;

#pragma region Define Objective Function
    // objective function
    IloExpr exp0(env);

    // N : la somme total des trips
    // M : le nombre total des vehicule

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < M; k++)
            {
                exp0 += C[i][j] * X[i][j][k];
            }
        }
    }

    Model.add(IloMinimize(env, exp0));

#pragma endregion
    cout << "Define objective function : DONE" << endl;

#pragma region Constraints

    // 1.a
    for (i = 1; i < N + 1; i++)
    {
        IloExpr exp2(env);
        for (k = 0; k < M; k++)
        {
            exp2 += Y[i][k];
        }
        Model.add(exp2 == IloTrue);
    }

    // 2.a
    IloExpr exp3(env);
    for (k = 0; k < M; k++)
    {
        exp3 += Y[0][k]; 
    }

    Model.add(exp3 <= M); 

    // 3.a
    for(i = 0; i<N+1; i++)
    {
        for(j=0; j<)
    }

#pragma endregion

    return 0;
}

float existCostFromDepotAndCalculate(string depotId, BusTrip bt, INTER_TRIPS interTrips)
{
    int duration = TargetInterTrip::findDurationByTargetId(bt.getBusStationDep()->getId(), interTrips.at(depotId));
    if (duration == -1)
    {
        return MAXFLOAT;
    }
    return duration * c_v;
}

float existCostFromTripToDepotAndCalculate(string depotId, BusTrip bt, INTER_TRIPS interTrips)
{
    int duration = TargetInterTrip::findDurationByTargetId(depotId, interTrips.at(bt.getBusStationArr()->getId()));
    if (duration == -1)
    {
        return MAXFLOAT;
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
    return MAXFLOAT;
}