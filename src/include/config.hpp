#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#define ROOT3 1.73205080757

#define INTERVAL_DURATION       1.0   // 1 hour
#define MWTOKW                  1000.0           // 1000 kW per 1 MW
#define KVTOV                   1000.0

#define PRESSURE_1ATM           101325.0
#define PRESSURE_ATM_TO_MM      0.0075
#define ATM_TO_MM               (PRESSURE_1ATM*PRESSURE_ATM_TO_MM)

#define M_TO_CM 100.0

#define SUBSTATIONS 6

#define RATED_220 220
#define RATED_500 500

#define PRECISION_QUANTIFIER double

#define PI 3.14159265359
#define ANGULAR_FREQUENCY (3.14159265359*200.0)

typedef PRECISION_QUANTIFIER length_t             ; // km
typedef PRECISION_QUANTIFIER voltage_t            ; // kV
typedef PRECISION_QUANTIFIER active_power_t       ; // MW
typedef PRECISION_QUANTIFIER reactive_power_t     ; // MVAr
typedef PRECISION_QUANTIFIER full_power_t         ; // MVA
typedef int                  wire_area_t          ; // mm^2
typedef PRECISION_QUANTIFIER radius_t             ; // cm
typedef PRECISION_QUANTIFIER active_resistance_t  ; // Ohm
typedef PRECISION_QUANTIFIER reactive_resistance_t; // Ohm
typedef PRECISION_QUANTIFIER temperature_t;       ; // *C above or below 0*C
typedef PRECISION_QUANTIFIER humidity_t           ; // relative humidity
typedef PRECISION_QUANTIFIER pressure_t           ; // relative pressure

enum VoltageClass
{
    VOLTAGE_220K = 0,
    VOLTAGE_330K,
    VOLTAGE_500K
};

enum InterpolationMode
{
    LINEAR = 0,
    TABLE
};

enum BusSystem
{
    SYSTEM1 = 0,
    SYSTEM2
};

enum WeatherType
{
    GOOD = 0,
    DRY_SNOW,
    RAIN,
    ICE
};

#endif // CONFIG_HPP_INCLUDED
