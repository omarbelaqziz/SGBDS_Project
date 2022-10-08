# Bus Stations process optimization 

## Description 
The following project is a solution to real bus network optimaztion 

## How to use the Repo 
### for visual studio users 
run
```
git clon https://github.com/mo-hunter123/SGBDS-project
# then just run the app

```

### for linux users 
you can configure a cmake file to run the app 

but in my usecase I used a bash script as the following : 

in the root directory run: 
```
touch build-proj.sh
```
then copy the script: 
```
#!/bin/bash 

g++  \
    ./SGBDS_Project/SGBDS_Project.cpp \
    ./SGBDS_Project/StringsOperations.cpp \
    ./SGBDS_Project/BusStation.cpp \
    ./SGBDS_Project/BusStationsHandler.cpp \
    ./SGBDS_Project/BusTripsHandler.cpp \
    ./SGBDS_Project/BusTrip.cpp \
    ./SGBDS_Project/BusInterTripsHandler.cpp \
    ./SGBDS_Project/StatisticsCalculator.cpp \
    ./SGBDS_Project/GraphGenerator.cpp \
    -o out-exec

```

then run 
```
chmod 755 build-proj.sh
./build-proj.sh
```
then you can execute the out file ```./out-exec``` in that case 

and that's it
### Happy hacking 