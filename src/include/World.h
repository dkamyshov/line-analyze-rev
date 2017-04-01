#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>

#include "config.hpp"

#include "utils.h"
#include "literals.h"

#include "LineWireSI.h"

class LineInfo {
public:
    VoltageClass vc;
    wire_area_t wa;
    std::string label;
    length_t length;
    size_t busSystem;

    LineInfo(std::string src);
};

class World {
public:
    size_t systemsCount;
    std::string sourceFile;
    std::string outputFile;
    std::string wiresFile;
    WeatherType weather[12];
    humidity_t humidity[12];
    temperature_t temperature[12];
    pressure_t pressure[12];
    double voltageCoefficient = 1.0;
    std::vector<LineInfo> lines;
    LineWireInformation lwi;
    bool shouldDivide = false;
    bool verbose = false;
    bool compress = false;
    bool useFullLossPower = false;
    bool useLossesActive = false;
    InterpolationMode interpolationMode = LINEAR;

    World(const char *sourceFile);

    void parseWires();
};

#endif // WORLD_H_INCLUDED
