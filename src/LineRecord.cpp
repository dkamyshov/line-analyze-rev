#include "LineRecord.h"

#include <stdio.h>
#include <string>
#include <math.h>

#include "config.hpp"

#include "LineWireSI.h"

using namespace std;

LineRecord::LineRecord() {};

LineRecord::LineRecord(string label,
              active_power_t P, reactive_power_t Q,
              wire_area_t wa,
              length_t ll, voltage_t U,
              VoltageClass voltageClass,
              LineWireInformation *wi) {
    this->label = label;
    this->Q = Q;
    this->P = P;
    this->S = pow(pow(this->Q, 2.0) + pow(this->P, 2.0), 0.5);
    this->wa = wa;
    this->ll = ll;
    this->U = U;
    this->vc = voltageClass;

    lws = wi->getInformation(voltageClass, wa);

    if(this->U > 5) {
        this->dP = (pow(this->Q, 2) + pow(this->P, 2)) / pow(this->U, 2) * lws->r;
        this->dQ = (pow(this->Q, 2) + pow(this->P, 2)) / pow(this->U, 2) * lws->x;
    } else {
        this->dP = 0;
        this->dQ = 0;
    }
}

LineRecord::LineRecord(const LineRecord &o) {
    this->label = o.label;
    this->Q = o.Q;
    this->P = o.P;
    this->S = o.S;
    this->wa = o.wa;
    this->ll = o.ll;
    this->wr = o.wr;
    this->vc = o.vc;
    this->lws = o.lws;
}

LineRecord LineRecord::operator=(const LineRecord &o) {
    return LineRecord(o);
}
