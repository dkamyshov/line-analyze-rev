#ifndef ENTRY_H_INCLUDED
#define ENTRY_H_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>

#include <math.h>
#include <cmath>

#include "config.hpp"
#include "utils.h"

#include "DateTime.h"
#include "LineWireSI.h"
#include "World.h"
#include "LineRecord.h"

class Entry {
public:
    active_power_t intervalLossPower;
    active_power_t combinedTransmissionP;
    reactive_power_t combinedTransmissionQ;

    WeatherType weather_type;

    humidity_t    humidity;
    temperature_t temperature;
    pressure_t    relativePressure;

    DateTime dt;

    voltage_t *S;

    LineRecord **dp;

    size_t linesCount;

    World *w;


    Entry(std::string, World *);
    ~Entry();
};


#endif // ENTRY_H_INCLUDED
