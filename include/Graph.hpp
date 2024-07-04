#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <map>

// Forward declarations
struct Edge;
struct Vertex;

// Graph class to hold the flight route data
class Graph {
public:
    void addEdge(const std::string& source, const std::string& target, double weight, double fuel, double cost);
    std::vector<int> findShortestPathDijkstra(const std::string& start, const std::string& goal, const std::string& criteria) const;
    std::vector<int> findShortestPathAStar(const std::string& start, const std::string& goal, const std::string& criteria) const;
    double calculateHeuristic(const std::string& vertex, const std::string& goal, const std::string& criteria) const;  // For A* algorithm

    std::map<std::string, int> vertexMap;
    std::vector<Vertex> vertices;
};

#endif // GRAPH_HPP
