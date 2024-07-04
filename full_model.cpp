#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <limits>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Struct to hold flight data
struct FlightData {
    string source;
    string destination;
    double distance;
    double fuelConsumption; // Additional attribute
    double cost;            // Additional attribute
};

// Function to read flight data from CSV file
vector<FlightData> readFlightDataFromCSV(const string& filename) {
    vector<FlightData> flightData;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return flightData;
    }

    string line;
    getline(file, line);  // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string source, destination, distanceStr, fuelStr, costStr;
        getline(ss, source, ',');
        getline(ss, destination, ',');
        getline(ss, distanceStr, ',');
        getline(ss, fuelStr, ',');
        getline(ss, costStr, ',');

        double distance, fuel, cost;
        try {
            distance = stod(distanceStr);
            fuel = stod(fuelStr);
            cost = stod(costStr);
        } catch (const std::invalid_argument& e) {
            cerr << "Invalid value found in CSV." << endl;
            continue;
        } catch (const std::out_of_range& e) {
            cerr << "Value out of range found in CSV." << endl;
            continue;
        }

        flightData.push_back({source, destination, distance, fuel, cost});
    }

    file.close();
    return flightData;
}

// Struct for graph edges
struct Edge {
    int target;
    double weight;
};

// Struct for graph vertices
struct Vertex {
    string code;
    vector<Edge> edges;
};

// Graph class to hold the flight route data
class Graph {
public:
    void addEdge(const string& source, const string& target, double weight, double fuel, double cost);
    vector<int> findShortestPathDijkstra(const string& start, const string& goal, const string& criteria) const;
    vector<int> findShortestPathAStar(const string& start, const string& goal, const string& criteria) const;
    double calculateHeuristic(const string& vertex, const string& goal, const string& criteria) const;  // For A* algorithm

    map<string, int> vertexMap;
    vector<Vertex> vertices;
};

void Graph::addEdge(const string& source, const string& target, double weight, double fuel, double cost) {
    if (vertexMap.find(source) == vertexMap.end()) {
        vertexMap[source] = vertices.size();
        vertices.push_back({source, {}});
    }
    if (vertexMap.find(target) == vertexMap.end()) {
        vertexMap[target] = vertices.size();
        vertices.push_back({target, {}});
    }
    int sourceIndex = vertexMap[source];
    int targetIndex = vertexMap[target];
    vertices[sourceIndex].edges.push_back({targetIndex, weight});
}

vector<int> Graph::findShortestPathDijkstra(const string& start, const string& goal, const string& criteria) const {
    vector<double> distances(vertices.size(), numeric_limits<double>::infinity());
    vector<int> predecessors(vertices.size(), -1);
    vector<bool> visited(vertices.size(), false);

    auto compare = [&distances](int lhs, int rhs) {
        return distances[lhs] > distances[rhs];
    };
    priority_queue<int, vector<int>, decltype(compare)> queue(compare);

    int startIndex = vertexMap.at(start);
    int goalIndex = vertexMap.at(goal);
    distances[startIndex] = 0;
    queue.push(startIndex);

    while (!queue.empty()) {
        int current = queue.top();
        queue.pop();

        if (visited[current]) continue;
        visited[current] = true;

        if (current == goalIndex) break;

        for (const Edge& edge : vertices[current].edges) {
            int neighbor = edge.target;
            double newDist = distances[current] + edge.weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                predecessors[neighbor] = current;
                queue.push(neighbor);
            }
        }
    }

    vector<int> path;
    for (int at = goalIndex; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

double Graph::calculateHeuristic(const string& vertex, const string& goal, const string& criteria) const {
    // Example heuristic: Euclidean distance between the vertices (could be based on lat/long if available)
    // For simplicity, using string length difference as a dummy heuristic
    if (criteria == "reduce_time") {
        return abs(static_cast<int>(vertex.length() - goal.length()));
    } else if (criteria == "reduce_fuel") {
        // Dummy heuristic for fuel consumption
        return abs(static_cast<int>(vertex.length() - goal.length())) * 10; // Adjust as per actual heuristics
    } else if (criteria == "reduce_cost") {
        // Dummy heuristic for cost
        return abs(static_cast<int>(vertex.length() - goal.length())) * 20; // Adjust as per actual heuristics
    } else {
        return 0;
    }
}

vector<int> Graph::findShortestPathAStar(const string& start, const string& goal, const string& criteria) const {
    vector<double> distances(vertices.size(), numeric_limits<double>::infinity());
    vector<double> heuristics(vertices.size(), numeric_limits<double>::infinity());
    vector<int> predecessors(vertices.size(), -1);
    vector<bool> visited(vertices.size(), false);

    auto compare = [&distances, &heuristics](int lhs, int rhs) {
        return distances[lhs] + heuristics[lhs] > distances[rhs] + heuristics[rhs];
    };
    priority_queue<int, vector<int>, decltype(compare)> queue(compare);

    int startIndex = vertexMap.at(start);
    int goalIndex = vertexMap.at(goal);
    distances[startIndex] = 0;
    heuristics[startIndex] = calculateHeuristic(start, goal, criteria);
    queue.push(startIndex);

    while (!queue.empty()) {
        int current = queue.top();
        queue.pop();

        if (visited[current]) continue;
        visited[current] = true;

        if (current == goalIndex) break;

        for (const Edge& edge : vertices[current].edges) {
            int neighbor = edge.target;
            double newDist = distances[current] + edge.weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                heuristics[neighbor] = calculateHeuristic(vertices[neighbor].code, goal, criteria);
                predecessors[neighbor] = current;
                queue.push(neighbor);
            }
        }
    }

    vector<int> path;
    for (int at = goalIndex; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void userInterface(const Graph& g) {
    cout << "Enter source airport code: ";
    string source;
    cin >> source;

    cout << "Enter destination airport code: ";
    string target;
    cin >> target;

    if (g.vertexMap.find(source) == g.vertexMap.end() || g.vertexMap.find(target) == g.vertexMap.end()) {
        cout << "Error: One or both of the entered airport codes do not exist in the graph." << endl;
        return;
    }

    cout << "Enter optimization criteria:" << endl;
    cout << "  1. Reduce travel time" << endl;
    cout << "  2. Reduce fuel consumption" << endl;
    cout << "  3. Reduce costs" << endl;
    cout << "Choose (1/2/3): ";
    int choice;
    cin >> choice;

    string criteria;
    switch (choice) {
        case 1:
            criteria = "reduce_time";
            break;
        case 2:
            criteria = "reduce_fuel";
            break;
        case 3:
            criteria = "reduce_cost";
            break;
        default:
            cout << "Invalid choice. Using default criteria (reduce_time)." << endl;
            criteria = "reduce_time";
            break;
    }

    vector<int> optimizedPath;
    if (criteria == "reduce_time") {
        optimizedPath = g.findShortestPathDijkstra(source, target, criteria);
    } else {
        optimizedPath = g.findShortestPathAStar(source, target, criteria);
    }

    if (optimizedPath.empty() || optimizedPath.size() == 1) {
        cout << "No valid path found between " << source << " and " << target << endl;
    } else {
        cout << "Optimized path:";
        for (int v : optimizedPath) {
            cout << " " << g.vertices[v].code;
        }
        cout << endl;

        // Calculate and display total distance
        double totalDistance = 0.0;
        for (size_t i = 0; i < optimizedPath.size() - 1; ++i) {
            int current = optimizedPath[i];
            int next = optimizedPath[i + 1];
            for (const auto& edge : g.vertices[current].edges) {
                if (edge.target == next) {
                    totalDistance += edge.weight;
                    break;
                }
            }
        }
        cout << "Total distance: " << totalDistance << " km" << endl;

        // Display additional criteria results
        if (criteria == "reduce_fuel" || criteria == "reduce_cost") {
            double totalValue = 0.0;
            for (size_t i = 0; i < optimizedPath.size() - 1; ++i) {
                int current = optimizedPath[i];
                int next = optimizedPath[i + 1];
                for (const auto& edge : g.vertices[current].edges) {
                    if (edge.target == next) {
                        if (criteria == "reduce_fuel") {
                            totalValue += g.vertices[current].code.length() * edge.weight; // Dummy calculation for fuel consumption
                        } else if (criteria == "reduce_cost") {
                            totalValue += g.vertices[current].code.length() * edge.weight; // Dummy calculation for cost
                        }
                        break;
                    }
                }
            }
            cout << "Additional value: " << totalValue << endl;
        }
    }
}

int main() {
    vector<FlightData> flightData = readFlightDataFromCSV("D:\\OneDrive\\Desktop\\Flight Optimization System Project\\flight_data.csv");
    if (flightData.empty()) {
        cerr << "No flight data was read. Exiting." << endl;
        return 1;
    }
    
    Graph g;
    // Add edges to the graph
    for (const auto& flight : flightData) {
        g.addEdge(flight.source, flight.destination, flight.distance, flight.fuelConsumption, flight.cost);
    }

    userInterface(g);
    return 0;
}
