#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <map>

struct Edge {
    int target;
    double distance;
    double fuelConsumption;  // Added attribute for fuel consumption
    double cost;             // Added attribute for cost
};

class Graph {
public:
    void addEdge(const std::string& source, const std::string& target, double distance, double fuelConsumption, double cost);
    std::vector<int> findShortestPathDijkstra(const std::string& start, const std::string& goal) const;
    std::vector<int> findOptimalPathByFuelConsumption(const std::string& start, const std::string& goal) const;
    std::vector<int> findOptimalPathByCost(const std::string& start, const std::string& goal) const;
    bool hasAirport(const std::string& airportCode) const;
    std::string getAirportCode(int index) const;
    std::vector<Edge> getEdges(int index) const;

private:
    std::map<std::string, int> vertexMap;
    std::vector<std::vector<Edge>> adjacencyList;
};

#endif // GRAPH_HPP
