#ifndef LINEWIRESI_H_INCLUDED
#define LINEWIRESI_H_INCLUDED

#include <stdlib.h>
#include <vector>

#include "config.hpp"

class LineWireSpecification {
public:
    VoltageClass voltageClass;

    wire_area_t             area; // mm^2
    active_resistance_t     r;    // Ohm/km
    reactive_resistance_t   x;    // Ohm/km

    size_t n;                     // wires in phase

    radius_t wr;                  // single wire radius (cm)
    radius_t wr_eq;

    double Dav, beta, A;

    double b0, C0; // Sm per km, F per km
    double kn;

    LineWireSpecification(VoltageClass,
                          wire_area_t,
                          active_resistance_t,
                          reactive_resistance_t,
                          size_t,
                          radius_t,
                          double,
                          double);
};

class LineWireInformation {
public:
    std::vector<LineWireSpecification> storage;

    LineWireInformation();
    void addWire(VoltageClass, wire_area_t, active_resistance_t, reactive_resistance_t, size_t, radius_t, double, double);
    LineWireSpecification *getInformation(VoltageClass vc, wire_area_t wa);
};

#endif // LINEWIRESI_H_INCLUDED
