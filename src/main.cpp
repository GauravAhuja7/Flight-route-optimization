#include <iostream>
#include <fstream>
#include "FlightData.hpp"
#include "Graph.hpp"
#include "Utils.hpp"

void userInterface(const Graph& g);

int main() {
    // Read flight data from CSV
    std::vector<FlightData> flightData = readFlightDataFromCSV("src/flight_data.csv");
    if (flightData.empty()) {
        std::cerr << "No flight data was read. Exiting." << std::endl;
        return 1;
    }
    
    // Create a Graph object and add edges
    Graph g;
    for (const auto& flight : flightData) {
        g.addEdge(flight.source, flight.destination, flight.distance, flight.fuelConsumption, flight.cost);
    }

    // Call user interface function
    userInterface(g);

    return 0;
}

void userInterface(const Graph& g) {
    std::cout << "Flight Route Optimization System" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    std::string source, destination;
    std::cout << "Enter source airport code: ";
    std::cin >> source;

    std::cout << "Enter destination airport code: ";
    std::cin >> destination;

    if (!g.hasAirport(source) || !g.hasAirport(destination)) {
        std::cerr << "Error: One or both of the entered airport codes do not exist in the graph." << std::endl;
        return;
    }

    // Prompt user for optimization criteria
    std::cout << "Select optimization criteria:" << std::endl;
    std::cout << "1. Reduce travel time" << std::endl;
    std::cout << "2. Reduce fuel consumption" << std::endl;
    std::cout << "3. Reduce costs (balance time and fuel)" << std::endl;

    int choice;
    std::cout << "Enter your choice (1-3): ";
    std::cin >> choice;

    std::vector<int> optimizedPath;
    switch (choice) {
        case 1:
            optimizedPath = g.findShortestPathDijkstra(source, destination);
            break;
        case 2:
            optimizedPath = g.findOptimalPathByFuelConsumption(source, destination);
            break;
        case 3:
            optimizedPath = g.findOptimalPathByCost(source, destination);
            break;
        default:
            std::cerr << "Invalid choice. Using Dijkstra's algorithm by default." << std::endl;
            optimizedPath = g.findShortestPathDijkstra(source, destination);
            break;
    }

    if (optimizedPath.empty() || optimizedPath.size() == 1) {
        std::cout << "No valid path found between " << source << " and " << destination << std::endl;
    } else {
        std::cout << "Optimized path:";
        for (int v : optimizedPath) {
            std::cout << " " << g.getAirportCode(v);
        }
        std::cout << std::endl;

        // Calculate and display total distance, fuel consumption, and cost
        double totalDistance = 0.0;
        double totalFuelConsumption = 0.0;
        double totalCost = 0.0;
        for (size_t i = 0; i < optimizedPath.size() - 1; ++i) {
            int current = optimizedPath[i];
            int next = optimizedPath[i + 1];
            for (const auto& edge : g.getEdges(current)) {
                if (edge.target == next) {
                    totalDistance += edge.distance;
                    totalFuelConsumption += edge.fuelConsumption;
                    totalCost += edge.cost;
                    break;
                }
            }
        }
        std::cout << "Total distance: " << totalDistance << " km" << std::endl;
        std::cout << "Total fuel consumption: " << totalFuelConsumption << " units" << std::endl;
        std::cout << "Total cost: $" << totalCost << std::endl;
    }
}
