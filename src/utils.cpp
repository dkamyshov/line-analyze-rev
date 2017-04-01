#include "config.hpp"

#include "utils.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "literals.h"

using namespace std;

template<typename ... Args>
string stringFormat( const string& format, Args ... args ) {
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 );
}

WeatherType parseWeatherType(string wt) {
    if(wt.compare(LITERAL_WEATHER_GOOD) == 0)       return GOOD;
    if(wt.compare(LITERAL_WEATHER_DRY_SNOW) == 0)   return DRY_SNOW;
    if(wt.compare(LITERAL_WEATHER_RAIN) == 0)       return RAIN;
    if(wt.compare(LITERAL_WEATHER_ICE) == 0)        return ICE;

    return GOOD;
}

VoltageClass parseVoltageClass(string vc) {
    if(vc.compare(LITERAL_VOLTAGE_220) == 0) return VOLTAGE_220K;
    if(vc.compare(LITERAL_VOLTAGE_330) == 0) return VOLTAGE_330K;
    if(vc.compare(LITERAL_VOLTAGE_500) == 0) return VOLTAGE_500K;

    return VOLTAGE_220K;
}

InterpolationMode parseInterpolationMode(string im) {
    if(im.compare(LITERAL_INTERPOLATION_LINEAR) == 0) return LINEAR;
    if(im.compare(LITERAL_INTERPOLATION_TABLE) == 0) return TABLE;

    return LINEAR;
}

string floatFormat(double n, size_t prec) {
    char tbuf[64], obuf[64];
    sprintf(tbuf, "%%.%uf", prec);
    sprintf(obuf, tbuf, n);
    return string(obuf);
}

string weatherTypeToString(WeatherType wt) {
    switch(wt) {
        case GOOD:          return string(LITERAL_WEATHER_GOOD);
        case DRY_SNOW:      return string(LITERAL_WEATHER_DRY_SNOW);
        case RAIN:          return string(LITERAL_WEATHER_RAIN);
        case ICE:           return string(LITERAL_WEATHER_ICE);
        default:            return string(LITERAL_ERROR);
    }
}

vector<string> tokenize(string src) {
    vector<std::string> result;
    string current("");

    for(size_t i = 0; i < src.size(); ++i) {
        char c = src.at(i);

        if(c == '\t' || c == ' ') {
            if(current.size() > 0) {
                transform(current.begin(), current.end(), current.begin(), ::toupper);
                result.push_back(current);
                current = "";
            }
        } else {
            current += c;
        }
    }

    if(current.size() > 0) result.push_back(current);

    return result;
}

std::vector<std::string> split(std::string src, char delimiter) {
    std::vector<std::string> result;
    std::string current("");

    for(size_t i = 0; i < src.size(); ++i) {
        if(src.at(i) == delimiter) {
            result.push_back(current);
            current = std::string("");
        } else {
            current = current + src.at(i);
        }
    }

    result.push_back(current);
    return result;
}

/*float stof(string in) {
    return (float) stod(in);
}

double stod(string in) {
    double a;

    sscanf(in.c_str(), "%lf", &a);

    return a;
}

int stoi(string in) {
    int a;

    sscanf(in.c_str(), "%d", &a);

    return a;
}

unsigned long stoul(string in) {
    unsigned long a;

    sscanf(in.c_str(), "%u", &a);

    return a;
}*/
