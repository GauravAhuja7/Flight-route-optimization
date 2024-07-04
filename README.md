# Flight Route Optimization System

This project aims to optimize flight routes between airports using data sourced from OpenFlight.org. It implements various algorithms to achieve different optimization objectives: reducing travel time, minimizing fuel consumption, and balancing operational costs.

## Objectives

- **Reduce Travel Time:** Optimize routes to ensure the shortest travel time.
- **Reduce Fuel Consumption:** Optimize routes to minimize fuel usage.
- **Reduce Costs:** Balance between time and fuel to minimize operational costs.

## Data Source

The flight data used in this project is sourced from [OpenFlight.org](https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat). It includes information about airports and flight connections worldwide, essential for calculating optimal routes.

## Algorithms Used

- **Dijkstra's Algorithm:** Used to find the shortest path based on distance.
- **A* Algorithm:** Implemented with a heuristic to find optimal paths considering both distance and a selected optimization criterion (time, fuel, or cost).

## Results

The system provides optimized flight routes based on user-selected criteria:

- **Shortest Travel Time:** Finds the path with the minimum distance.
- **Minimized Fuel Consumption:** Considers routes that consume the least amount of fuel.
- **Optimized Costs:** Balances between time and fuel costs to provide cost-effective routes.

## Usage
(PS: You can simply look at the full_model.cpp and don't get into complexities or you can just look at the standard way)
To use this repository:

1. **Clone the Repository:**
git clone (https://github.com/GauravAhuja7/Flight-route-optimization)
cd flight-route-optimization


2. **Compile and Run:**
- Ensure you have a C++ compiler installed (e.g., g++).
- Compile the code:
  ```
  g++ -o flight_optimization src/main.cpp
  ```
- Run the executable:
  ```
  ./flight_optimization
  ```

3. **Input Instructions:**
- Enter the source and destination airport codes as prompted.
- Choose optimization criteria (1-3) based on your preference (reduce travel time, reduce fuel consumption, or reduce costs).

4. **View Results:**
- The program will display the optimized route, total distance, fuel consumption, and cost based on your selection.
