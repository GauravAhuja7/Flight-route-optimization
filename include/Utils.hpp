#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include "FlightData.hpp"

// Function to read flight data from CSV file
std::vector<FlightData> readFlightDataFromCSV(const std::string& filename);

#endif // UTILS_HPP
