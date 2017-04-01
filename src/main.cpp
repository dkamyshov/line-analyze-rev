#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <exception>

#include "config.hpp"
#include "utils.h"
#include "Linterp.h"
#include "DateTime.h"
#include "LineRecord.h"
#include "Entry.h"
#include "World.h"
#include "output.h"

#include "xlsxwriter.h"

using namespace std;

double WReq(double n, double a, double WR) {
    return pow(pow(a, n-1)*WR, 1.0/n);
}

LinterpStorage good(10);
LinterpStorage dry_snow(10);
LinterpStorage rain(10);
LinterpStorage ice(10);

void fillLinterp() {
    good.put(0, 0);
    good.put(0.55, 0.02);
    good.put(0.60, 0.035);
    good.put(0.65, 0.06);
    good.put(0.70, 0.09);
    good.put(0.75, 0.14);
    good.put(0.80, 0.21);
    good.put(0.85, 0.35);
    good.put(0.90, 0.6);
    good.put(0.95, 0.9);

    dry_snow.put(0, 0);
    dry_snow.put(0.55, 0.07);
    dry_snow.put(0.60, 0.12);
    dry_snow.put(0.65, 0.18);
    dry_snow.put(0.70, 0.3);
    dry_snow.put(0.75, 0.45);
    dry_snow.put(0.80, 0.7);
    dry_snow.put(0.85, 1.2);
    dry_snow.put(0.90, 1.8);
    dry_snow.put(0.95, 2.75);

    rain.put(0, 0);
    rain.put(0.55, 0.2);
    rain.put(0.60, 0.375);
    rain.put(0.65, 0.68);
    rain.put(0.70, 1.05);
    rain.put(0.75, 1.6);
    rain.put(0.80, 2.3);
    rain.put(0.85, 3.25);
    rain.put(0.90, 4.0);
    rain.put(0.95, 5.2);

    ice.put(0, 0);
    ice.put(0.55, 0.8);
    ice.put(0.60, 1.4);
    ice.put(0.65, 2.25);
    ice.put(0.70, 3.5);
    ice.put(0.75, 4.75);
    ice.put(0.80, 6.5);
    ice.put(0.85, 8.5);
    ice.put(0.90, 11);
    ice.put(0.95, 13);
}

/*
    KW per KM
*/
double lossesInterpolated(World *w, WeatherType wt, double x) {
    switch(w->interpolationMode) {
        case LINEAR:
            switch(wt) {
                case GOOD:      return (0.875*x - 0.47125)/0.4;
                case DRY_SNOW:  return (2.68*x  - 1.446)/0.4;
                case RAIN:      return (4.9*x  - 2.615)/0.4;
                case ICE:       return (13.1*x  - 6.845)/0.4;
                default:        return 0;
            }
        case TABLE:
            switch(wt) {
                case GOOD:      return good.interpolate(x);
                case DRY_SNOW:  return dry_snow.interpolate(x);
                case RAIN:      return rain.interpolate(x);
                case ICE:       return ice.interpolate(x);
                default:        return 0;
            }
        default: return 0;
    }
}

/*
    Combined losses in MW/km
*/
active_power_t intervalLossPower(Entry *e) {
    #define M 0.9

    double D = (0.386 * e->relativePressure * ATM_TO_MM) / (273 + e->temperature);

    active_power_t result = 0;

    for(size_t i = 0; i < e->linesCount; ++i) {
        LineRecord *dp = e->dp[i];

        voltage_t Uexl = dp->U;

        if(Uexl > 5) {
            voltage_t E0, E, Eeq;
            active_power_t dPq = 0;

            switch(dp->vc) {
                case VOLTAGE_220K:
                    // kV per cm
                    E0 = 24.5 * M * D * (1 / pow(dp->lws->wr * D, 0.4));
                    // kV per cm
                    E = 0.0147 * (dp->lws->C0 * 1e12 / 1e3) * dp->U / (dp->lws->n * dp->lws->wr);
                    Eeq = (1 + dp->lws->kn)/2*E;

                    if(e->w->shouldDivide) {
                        // to MW
                        dPq = lossesInterpolated(e->w, e->weather_type, Eeq/E0) / (dp->lws->n * pow(dp->lws->wr, 2)) / 1000.0;
                    } else {
                        dPq = lossesInterpolated(e->w, e->weather_type, Eeq/E0) / 1000.0;
                    }
                break;

                case VOLTAGE_500K:
                    // kV per cm
                    E0 = 24.5 * M * D * (1 / pow(dp->lws->wr * D, 0.4));
                    // kV per cm
                    E = 0.0147 * (dp->lws->C0 * 1e12 / 1e3) * dp->U / (dp->lws->n * dp->lws->wr);
                    Eeq = (1 + dp->lws->kn)/2*E;

                    if(e->w->shouldDivide) {
                        // to MW
                        dPq = lossesInterpolated(e->w, e->weather_type, Eeq/E0) / (dp->lws->n * pow(dp->lws->wr, 2)) / 1000.0;
                    } else {
                        dPq = lossesInterpolated(e->w, e->weather_type, Eeq/E0) / 1000.0;
                    }
                break;
            }

            result += dPq; // MW per km
            dp->ilp = dPq; // MW per km
        } else {
            dp->ilp = 0;
        }
    }

    return result;
}

vector<Entry*> parseSource(World *w) {
    ifstream infile(w->sourceFile);

    vector<Entry*> result;

    string line;
    while(getline(infile, line)) {
        Entry *e = new Entry(line, w);
        e->intervalLossPower = intervalLossPower(e);
        result.push_back(e);
    }

    return result;
}

int main(int argc, char ** argv) {
    fillLinterp();

    World w(argv[1]);

    printf("Line Analyzer - Revised, version 3.0, 2016. Danil Kamyshov.\nUsing description: %s\nInput: %s\nOutput: %s\n%s\n\n",
           argv[1],
           w.sourceFile.c_str(),
           w.outputFile.c_str(),
           w.compress ? "Using short output." : "Using full output.");

    vector<Entry*> entries = parseSource(&w);
    outputFile(entries, &w);

    return 0;
}
