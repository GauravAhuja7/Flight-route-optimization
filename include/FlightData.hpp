#ifndef FLIGHTDATA_HPP
#define FLIGHTDATA_HPP

#include <string>

// Struct to hold flight data
struct FlightData {
    std::string source;
    std::string destination;
    double distance;
    double fuelConsumption;
    double cost;
};

#endif // FLIGHTDATA_HPP
