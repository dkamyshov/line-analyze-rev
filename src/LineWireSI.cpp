#include "LineWireSI.h"

#include <stdlib.h>
#include <vector>
#include <math.h>

#include "config.hpp"

using namespace std;

LineWireSpecification::LineWireSpecification(VoltageClass vc,
                      wire_area_t wa,
                      active_resistance_t r,
                      reactive_resistance_t x,
                      size_t n,
                      radius_t wr,
                      double Dav,
                      double A)
{
    this->voltageClass = vc;
    this->area = wa;
    this->r = r/n;
    this->n = n;
    this->wr = wr;
    this->Dav = Dav;

    this->beta = 2*(n-1)*sin(PI/n);
    this->wr_eq = pow(pow(A, n-1), 1.0/n);

    if(n == 1) {
        this->x = x;
        this->b0 = 7.58 / log10(Dav / this->wr) * 1e-6;
    } else {
        this->x = 0.144 * log10(Dav / this->wr_eq) + 0.016 / n * 8.5e-4;
        this->b0 = 7.58 / log10(Dav / this->wr_eq) * 1e-6;
    }

    this->C0 = this->b0 / ANGULAR_FREQUENCY;

    this->kn = (n == 1) ? 1 : 1 + this->beta * this->wr_eq / A;
};

LineWireInformation::LineWireInformation() {}

void LineWireInformation::addWire(VoltageClass vc, wire_area_t wa, active_resistance_t r, reactive_resistance_t x, size_t wc, radius_t wr, double Dav, double A) {
    this->storage.push_back(LineWireSpecification(vc, wa, r, x, wc, wr, Dav, A));
}

LineWireSpecification *LineWireInformation::getInformation(VoltageClass vc, wire_area_t wa) {
    for(size_t i = 0; i < storage.size(); ++i) {
        LineWireSpecification *c = &storage.at(i);

        if(c->voltageClass == vc && c->area == wa) {
            return c;
        }
    }

    return nullptr;
}
