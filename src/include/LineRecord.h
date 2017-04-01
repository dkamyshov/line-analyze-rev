#ifndef DISCRETEPOWER_H_INCLUDED
#define DISCRETEPOWER_H_INCLUDED

#include <stdio.h>
#include <string>

#include "config.hpp"

#include "LineWireSI.h"

using namespace std;

class LineRecord {
public:
    VoltageClass vc;

    active_power_t      P;
    active_power_t      dP;
    active_power_t      ilp;
    reactive_power_t    Q;
    reactive_power_t    dQ;
    full_power_t        S;

    voltage_t           U;

    radius_t wr;
    length_t ll;
    wire_area_t wa;

    LineWireSpecification *lws;

    string label;

    LineRecord();
    LineRecord(string, active_power_t,
                  reactive_power_t,
                  wire_area_t,
                  length_t, voltage_t,
                  VoltageClass,
                  LineWireInformation *);
    LineRecord(const LineRecord &);
    LineRecord operator=(const LineRecord &);
};

#endif // DISCRETEPOWER_H_INCLUDED
