#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "FlightData.hpp"

std::vector<FlightData> readFlightDataFromCSV(const std::string& filename);

#endif // UTILS_HPP
