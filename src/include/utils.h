#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "config.hpp"

#include <string>
#include <vector>

template<typename ... Args>
std::string stringFormat( const std::string& format, Args ... args );

WeatherType parseWeatherType(std::string);
VoltageClass parseVoltageClass(std::string);
InterpolationMode parseInterpolationMode(std::string);
std::string floatFormat(double, size_t);
std::string weatherTypeToString(WeatherType);
std::vector<std::string> tokenize(std::string);
std::vector<std::string> split(std::string, char);

// REMOVE LATER
/*float stof(std::string);
double stod(std::string);
int stoi(std::string);
unsigned long stoul(std::string);*/

#endif // UTILS_H_INCLUDED
