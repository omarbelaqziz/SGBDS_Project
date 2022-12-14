#include <iostream>

#include <stdio.h>
#include "../SGBDS_Project/preproc.h"
#include "ilcplex/ilocplex.h"

using namespace std;

#define NOT_EXIST_VALUE MAXFLOAT

typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;

float existCostFromDepotAndCalculate(string depotId, string target, INTER_TRIPS interTrips);
float existCostFromTripToDepotAndCalculate(string source, string depotId, INTER_TRIPS interTrips);
float existCostBetweenTwotTripsAndCalculate(BusTrip startTrip, BusTrip currentTrip, INTER_TRIPS interTrips);

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
    bool **A;
    int N = busTripsPopulation.size();
    int M = 3;

    C = (float **)malloc(sizeof(float *) * (N + 1));
    A = (bool **)malloc(sizeof(bool *) * (N + 1));

    for (int i = 0; i < N + 1; i++)
    {
        C[i] = (float *)malloc(sizeof(float) * (N + 1));
        A[i] = (bool *)malloc(sizeof(bool) * (N + 1));
    }

    int i = 0;
    int j = 0;
    int k = 0;

    cout << "Size : " << N << endl;

    multiset<BusTrip>::iterator it_to_i_trip;
    multiset<BusTrip>::iterator it_to_j_trip;

    C[0][0] = NOT_EXIST_VALUE;
    A[0][0] = false;

    for (i = 1, it_to_i_trip = busTripsPopulation.begin(); it_to_i_trip != busTripsPopulation.end(); ++i, ++it_to_i_trip)
    {

        C[i][0] = existCostFromTripToDepotAndCalculate((*it_to_i_trip).getBusStationArr()->getId(), depotId, interTrips);
        C[i][0] == NOT_EXIST_VALUE ? A[i][0] = false : A[i][0] = true;

        C[0][i] = existCostFromDepotAndCalculate(depotId, (*it_to_i_trip).getBusStationDep()->getId(), interTrips);
        C[0][i] == NOT_EXIST_VALUE ? A[0][i] = false : A[0][i] = true;

        for (j = 1, it_to_j_trip = busTripsPopulation.begin(); it_to_j_trip != busTripsPopulation.end(); ++it_to_j_trip, ++j)
        {
            if (i != j)
            {
                C[i][j] = existCostBetweenTwotTripsAndCalculate((*it_to_i_trip), (*it_to_j_trip), interTrips);
                C[i][j] == NOT_EXIST_VALUE ? A[i][j] = false : A[i][j] = true;
            }
            else
            {
                C[i][j] = NOT_EXIST_VALUE;
                A[i][j] = false;
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
    NumVar3D X(env, N + 1);
    NumVar2D Y(env, N + 1);

    IloNumVarArray U(env, N + 1, 0.0, IloInfinity, ILOFLOAT);

    for (i = 0; i < N + 1; i++)
    {
        Y[i] = IloNumVarArray(env, M, IloFalse, IloTrue, ILOBOOL);
    }

    for (i = 0; i < N + 1; i++)
    {
        X[i] = NumVar2D(env, N + 1);
        for (j = 0; j < N + 1; j++)
        {
            X[i][j] = IloNumVarArray(env, N + 1);
            for (k = 0; k < M; k++)
            {
                X[i][j][k] = IloNumVar(env, IloFalse, IloTrue, ILOBOOL);
            }
        }
    }

    // U = IloNumVarArray(env, N + 1, 0.0, IloNum, ILOFLOAT);

#pragma endregion
    cout << "Define decicion variables : DONE" << endl;

#pragma region Define Objective Function
    // objective function
    IloExpr exp0(env);

    // N : la somme total des trips
    // M : le nombre total des vehicule

    for (i = 0; i < N + 1; i++)
    {
        for (j = 0; j < N + 1; j++)
        {
            for (k = 0; k < M; k++)
            {
                if (A[i][j] == true)
                    exp0 += (C[i][j] * X[i][j][k]);
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
        Model.add(exp2 == 1);
    }

    // 2.a
    IloExpr exp3(env);
    for (k = 0; k < M; k++)
    {
        exp3 += Y[0][k];
    }

    Model.add(exp3 <= M);

    // 3.a
    for (i = 0; i < N + 1; i++)
    {
        for (k = 0; k < M; k++)
        {
            IloExpr exp4(env);
            IloExpr exp5(env);
            for (j = 0; j < N + 1; j++)
            {
                exp4 += (X[i][j][k] - Y[i][k]);
                exp5 += (X[j][i][k] - Y[i][k]);
            }
            Model.add(exp4 == exp5);
            Model.add(exp5 == 0);
        }
    }

     // subtour elimination
    for (k = 0; k < M; k++)
    {
        for (i = 1; i < N + 1; i++)
        {
            IloExpr exp6(env);
            for (j = 1; j < N + 1; j++)
            {
                if (i != j && A[i][j])
                {
                    exp6 += U[i] - U[j] + (N + 1) * X[i][j][k];
                }
            }
            Model.add(exp6 <= N);
        }
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
    cout << "D -> ";
    for (i = 0; i < N + 1; i++)
    {
        for (j = 0; j < N + 1; j++)
        {
            int ArcExist = cplex.getValue(X[i][j][0]);
            if (ArcExist)
            {
                if (A[i][j])
                    cout << j << " -> ";
            }
        }
    }

    cout << endl
         << "=----------------=" << endl;

    for (i = 0; i < N + 1; i++)
    {
        for (k = 0; k < M; k++)
        {
            int YPlex = cplex.getValue(Y[i][k]);
            cout << YPlex << ", ";
        }
        cout << endl;
    }

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