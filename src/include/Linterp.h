#ifndef LINTERP_H_INCLUDED
#define LINTERP_H_INCLUDED

#include "config.hpp"

class LinterpStorage {
public:
    size_t dp;
    size_t offset;
    double *x;
    double *y;

    double minX = 0;
    double maxX = 0;

    LinterpStorage(size_t dp) {
        this->dp = dp;
        this->x = new double[dp];
        this->y = new double[dp];
        this->offset = 0;
    }

    void put(double _x, double _y) {
        this->x[this->offset] = _x;
        this->y[this->offset] = _y;

        this->offset++;

        if(this->minX > _x) this->minX = _x;
        if(this->maxX < _x) this->maxX = _x;
    }

    double interpolate(double cx) {
        if(cx < this->minX) {
            return this->y[0] + (this->y[1] - this->y[0]) / (this->x[1] - this->x[0]) * (cx - this->x[0]);
        }

        if(cx > this->maxX) {
            return this->y[this->dp-2] + (this->y[this->dp-2] - this->y[this->dp-1]) / (this->x[this->dp-2] - this->x[this->dp-1]) * (cx - this->x[this->dp-1]);
        }

        for(size_t i = 0; i < this->dp-1; ++i) {
            /*double ccx = this->x[i];
            if(cx <= ccx) {
                double result = this->y[i-1] + (this->y[i] - this->y[i-1]) / (this->x[i] - this->x[i-1]) * (cx - this->x[i-1]);
                return result;
            }*/

            double ncx = this->x[i+1],
                   pcx = this->x[i];

            if((pcx <= cx) && (cx <= ncx)) {
                double result = this->y[i-1] + (this->y[i] - this->y[i-1]) / (this->x[i] - this->x[i-1]) * (cx - this->x[i-1]);
                return result;
            }
        }

        return 0;
    }
};

#endif // LINTERP_H_INCLUDED
