#include "DateTime.h"

#include "config.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

DateTime::DateTime() {}

DateTime::DateTime(string line) {
    sscanf(line.c_str(), "%u/%u/%u %u:%u:%u", &this->D, &this->M, &this->Y, &this->h, &this->m, &this->s);
}

string DateTime::toString() {
    char buf[64];
    sprintf(buf, "%u.%u.%u %u:%u:%u", this->D, this->M, this->Y, this->h, this->m, this->s);
    return string(buf);
}
