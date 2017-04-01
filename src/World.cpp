#include "World.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>

#include "config.hpp"

#include "utils.h"
#include "literals.h"

#include "LineWireSI.h"

using namespace std;

LineInfo::LineInfo(string src) {
    vector<string> info = tokenize(src);

    this->vc            = parseVoltageClass(info[1]);
    this->label         = info[2];
    this->wa            = stoi(info[3]);
    this->length        = stod(info[4]);
    this->busSystem     = stoi(info[5])-1;
}

World::World(const char *sourceFile) {
    ifstream infile(sourceFile);

    string line;
    while(getline(infile, line)) {
        if(line.size() == 0 || line[0] == '#') continue;

        vector<string> sline = tokenize(line);

        if(sline[0].compare(LITERAL_LOSSES_ACTIVE) == 0) {
            this->useLossesActive = true;
        }

        if(sline[0].compare(LITERAL_VERBOSE) == 0) {
            this->verbose = true;
        }

        if(sline[0].compare(LITERAL_COMPRESS) == 0) {
            this->compress = true;
        }

        if(sline[0].compare(LITERAL_LOSSES_FULL_POWER) == 0) {
            this->useFullLossPower = true;
        }

        if(sline[0].compare(LITERAL_INTERPOLATION) == 0) {
            this->interpolationMode = parseInterpolationMode(sline[1]);
        }

        if(sline[0].compare(LITERAL_DIVISION) == 0) {
            this->shouldDivide = (sline[1].compare(LITERAL_TRUE) == 0) ? true : false;
        }

        if(sline[0].compare(LITERAL_VOLTAGE) == 0) {
            this->voltageCoefficient = stod(sline[1]);
        }

        if(sline[0].compare(LITERAL_SYSTEMS) == 0) {
            this->systemsCount = stoul(sline[1]);
            if(this->verbose) printf("Systems count: %u\n", this->systemsCount);
        }

        if(sline[0].compare(LITERAL_SOURCE) == 0) {
            this->sourceFile = sline[1];
            if(this->verbose) printf("Source file: %s\n", this->sourceFile.c_str());
        }

        if(sline[0].compare(LITERAL_OUTPUT) == 0) {
            this->outputFile = sline[1];
            if(this->verbose) printf("Output file: %s\n", this->outputFile.c_str());
        }

        if(sline[0].compare(LITERAL_WIRES) == 0) {
            this->wiresFile = sline[1];
            if(this->verbose) printf("Wires file: %s\n", this->wiresFile.c_str());
        }

        if(sline[0].compare(LITERAL_WEATHER) == 0) {
            if(this->verbose) puts("Weather:");
            for(size_t i = 1; i < 13; ++i) {
                this->weather[i-1] = parseWeatherType(sline[i]);
                if(this->verbose) printf("%s ", weatherTypeToString(this->weather[i-1]).c_str());
            }
            if(this->verbose) puts("");
        }

        if(sline[0].compare(LITERAL_TEMPERATURE) == 0) {
            if(this->verbose) puts("Temperature:");
            for(size_t i = 1; i < 13; ++i) {
                this->temperature[i-1] = stod(sline[i]);
                if(this->verbose) printf("%.1f ", this->temperature[i-1]);
            }
            if(this->verbose) puts("");
        }

        if(sline[0].compare(LITERAL_HUMIDITY) == 0) {
            if(this->verbose) puts("Humidity:");
            for(size_t i = 1; i < 13; ++i) {
                this->humidity[i-1] = stod(sline[i]);
                if(this->verbose) printf("%.2f ", this->humidity[i-1]);
            }
            if(this->verbose) puts("");
        }

        if(sline[0].compare(LITERAL_PRESSURE) == 0) {
            if(this->verbose) puts("pressure:");
            for(size_t i = 1; i < 13; ++i) {
                this->pressure[i-1] = stod(sline[i]);
                if(this->verbose) printf("%.2f ", this->pressure[i-1]);
            }
            if(this->verbose) puts("");
        }

        if(sline[0].compare(LITERAL_LINE) == 0) {
            this->lines.push_back(LineInfo(line));
        }
    }

    if(this->verbose) printf("Lines: %u\n", this->lines.size());

    this->parseWires();
}

void World::parseWires() {
    ifstream infile(this->wiresFile);

    string line;
    while(getline(infile, line)) {
        if(line.size() == 0 || line[0] == '#') continue;

        vector<string> info = tokenize(line);

        VoltageClass vc             = parseVoltageClass(info[0]);
        wire_area_t wa              = stoi(info[1]);
        active_resistance_t r       = stod(info[2]);
        reactive_resistance_t x     = stod(info[3]);
        size_t wc                   = stoul(info[4]);
        radius_t wr                 = stod(info[5]);
        double Dav                  = stod(info[6]);
        double A                    = stod(info[7]);

        this->lwi.addWire(vc, wa, r, x, wc, wr, Dav, A);
    }
}
