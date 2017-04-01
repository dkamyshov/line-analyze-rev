#ifndef DATETIME_H_INCLUDED
#define DATETIME_H_INCLUDED

#include "config.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string>

class DateTime {
public:
    size_t h, m, s, D, Y, M;

    DateTime();

    DateTime(std::string line);

    std::string toString();
};

#endif // DATETIME_H_INCLUDED
