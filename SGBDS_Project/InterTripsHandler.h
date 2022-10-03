#pragma once
#include <set>
#include "InterTrip.h"


set<InterTrip*> handle_file_stream_inter_trips(ifstream& i_file, set<BusStation*> busStationsSet);


