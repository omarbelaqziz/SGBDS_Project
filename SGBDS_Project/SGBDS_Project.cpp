#include "preproc.h"
#include "GraphGenerator.h"

using namespace std;

BusStation createBusStationFromLine(string line)
{
    string fineLine(StringsOperations::removeLastChar(StringsOperations::ltrim(StringsOperations::rtrim(line))));
    vector<string> strings = StringsOperations::split(fineLine);
    return BusStation(strings[0], StringsOperations::stringToBool(strings[1]));
}

int main()
{
    std::clock_t c_start = std::clock();
    char filenames[][19] = {
        "../data/test50.txt",
        "../data/test54.txt",
        "../data/test55.txt",
        "../data/test56.txt",
        "../data/test57.txt",
        "../data/test58.txt",
        "../data/test59.txt",
        "../data/test60.txt"};

    char output_files[][20] = {
        "../output/out50.txt",
        "../output/out54.txt",
        "../output/out55.txt",
        "../output/out56.txt",
        "../output/out57.txt",
        "../output/out58.txt",
        "../output/out59.txt",
        "../output/out60.txt"};

    int data_set_index = 0;

    for (auto FILEE : filenames)
    {
        data_set_index++;
        string filename = FILEE;
        string outfile = output_files[data_set_index - 1];

        std::cout << "Handling data set N#: " << data_set_index << " / "
                  << "8" << endl;
        set<BusStation> *busStations = nullptr;

        INTER_TRIPS interTrips;
        TRIPS_MAP tripsStations;

        ifstream dataFile;
        ofstream dataOutFile;
        dataOutFile.open(outfile);
        dataFile.open(filename, ios::in);

        if (dataFile.is_open() && dataOutFile.is_open())
        {
            string line;
            int lineNumber = 0;
            while (getline(dataFile, line))
            {
                lineNumber++;
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
            showAllStatistics(tripsStations, dataOutFile);
            graph_generator(interTrips, busStations, tripsStations, dataOutFile);

            delete busStations;
        }
        else if (!dataFile.is_open())
        {
            std::cout << "cannot open " << filename << endl;
        }
        else
        {
            std::cout << "cannot open " << outfile << endl;
        }
        dataFile.close();
    }

    std::clock_t c_end = std::clock();
    long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;

    std::cout << "CPU time used: " << time_elapsed_ms / 1000.0 << " s" << endl;
}
