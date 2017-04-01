#include "Entry.h"

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

using namespace std;

Entry::Entry(string source, World *w) {
    this->linesCount = w->lines.size();

    this->w = w;

    this->dp = new LineRecord*[w->lines.size()];
    this->S = new voltage_t[w->systemsCount];

    this->combinedTransmissionP = 0;
    this->combinedTransmissionQ = 0;

    vector<string> c = split(source, ';');

    this->dt = DateTime(c.at(0));

    for(size_t i = 0; i < w->systemsCount; ++i) {
        this->S[i] = stod(c[i+1]) * w->voltageCoefficient;
    }

    for(size_t i = 0; i < w->lines.size(); ++i) {
        this->dp[i] = new LineRecord(w->lines.at(i).label,
                                        stod(c.at(1 + w->systemsCount + i*2)),
                                        stod(c.at(1 + w->systemsCount + i*2 + 1)),
                                        w->lines.at(i).wa,
                                        w->lines.at(i).length,
                                        this->S[w->lines.at(i).busSystem],
                                        w->lines.at(i).vc,
                                        &w->lwi);
    }

    for(size_t i = 0; i < w->lines.size(); ++i) {
        this->combinedTransmissionP += abs(this->dp[i]->P);
        this->combinedTransmissionQ += abs(this->dp[i]->Q);
    }

    this->weather_type  = w->weather[this->dt.M - 1];
    this->temperature   = w->temperature[this->dt.M-1];
    this->humidity      = w->humidity[this->dt.M-1];
    this->relativePressure = w->pressure[this->dt.M-1];
}

Entry::~Entry() {
     delete [] this->dp;
     delete [] this->S;
}
