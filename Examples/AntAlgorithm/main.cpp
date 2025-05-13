#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <limits>
#include <iostream>
#include <cmath> // For std::pow

// Include Boost Graph Library headers
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp> // Required for boost::get

// Define the graph type using Boost.
// listS: use std::list to store edges per vertex (allows easy removal)
// vecS: use std::vector to store vertices (allows vertex descriptors to be used as indices)
// directedS: directed graph (TSP is often symmetric, but directed allows one-way costs if needed)
// property<boost::edge_weight_t, double>: add a property to edges to store a double weight
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                              boost::no_property, boost::property<boost::edge_weight_t, double>> Graph;

// Define descriptors for vertices and edges
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

class Ant {
public:
    std::vector<int> tour;
    double tour_cost;
    std::vector<bool> visited;
    int current_vertex;
    int num_vertices;

    Ant(int n_vertices, int start_vertex) :
        tour(1, start_vertex),
        tour_cost(0.0),
        visited(n_vertices, false),
        current_vertex(start_vertex),
        num_vertices(n_vertices)
    {
        visited[start_vertex] = true;
    }

    // Move the ant to the next vertex
    // graph: the graph structure
    // pheromones: the pheromone matrix
    // alpha: pheromone influence factor
    // beta: heuristic influence factor
    // rng: random number generator
    bool move_to_next_vertex(const Graph& graph, const std::vector<std::vector<double>>& pheromones,
                             double alpha, double beta, std::mt19937& rng);

    // Calculate the cost of the completed tour
    // graph: the graph structure
    void calculate_tour_cost(const Graph& graph);

    // Reset the ant for a new iteration
    void reset(int start_vertex);
};

class ACO_Solver {
private:
    Graph graph;
    int num_vertices;
    int num_ants;
    int num_iterations;
    double evaporation_rate;
    double alpha; // Pheromone influence
    double beta;  // Heuristic influence
    double initial_pheromone; // Initial pheromone level

    std::vector<std::vector<double>> pheromones;
    std::vector<int> best_tour;
    double best_tour_cost;

    std::mt19937 rng; // Random number generator

public:
    ACO_Solver(const Graph& g, int ants, int iterations, double evap_rate, double a, double b, double initial_phero);

    // Main function to run the ACO algorithm
    void run();

    // Get the best tour found
    const std::vector<int>& get_best_tour() const {
        return best_tour;
    }

    // Get the cost of the best tour found
    double get_best_tour_cost() const {
        return best_tour_cost;
    }

private:
    // Initializes the pheromone trails
    void initialize_pheromones();

    // Constructs tours for all ants in an iteration
    void construct_tours(std::vector<Ant>& ants);

    // Updates the pheromone trails based on the completed tours
    void update_pheromones(const std::vector<Ant>& ants);

    // Calculate the desirability (pheromone * heuristic) of moving to a neighbor
    double calculate_desirability(int u, int v, const Graph& graph, const std::vector<std::vector<double>>& pheromones, double alpha, double beta) const;

    // Deposit pheromones on a path
    void deposit_pheromone(const std::vector<int>& tour, double pheromone_amount);
};

// --- Ant Class Implementation ---

bool Ant::move_to_next_vertex(const Graph& graph, const std::vector<std::vector<double>>& pheromones,
                              double alpha, double beta, std::mt19937& rng) {
    std::vector<int> unvisited_neighbors;
    for (int i = 0; i < num_vertices; ++i) {
        if (!visited[i]) {
             // Check if an edge exists from current_vertex to i
             if (boost::edge(current_vertex, i, graph).second) {
                 unvisited_neighbors.push_back(i);
             }
        }
    }

    if (unvisited_neighbors.empty()) {
        // All cities visited. Attempt to return to the starting vertex.
        if (tour.size() == num_vertices) {
             int start_vertex = tour[0];
             if (boost::edge(current_vertex, start_vertex, graph).second) {
                 tour.push_back(start_vertex);
                 // current_vertex remains the same until reset, cost calculated later
                 return true; // Successfully identified path back to start
             } else {
                 // Cannot return to start, path is invalid for TSP
                 tour_cost = std::numeric_limits<double>::max(); // Mark as invalid
                 return false;
             }
        }
        // Should not happen if num_vertices > 0
        return false; // Cannot move
    }

    // Calculate probabilities for moving to unvisited neighbors
    std::vector<double> probabilities;
    double total_desirability = 0.0;

    for (int neighbor_vertex : unvisited_neighbors) {
        Edge e; bool exists;
        boost::tie(e, exists) = boost::edge(current_vertex, neighbor_vertex, graph);
        // Access edge weight directly using boost::get
        double distance = exists ? boost::get(boost::edge_weight, graph, e) : std::numeric_limits<double>::infinity();

        // Avoid division by zero if distance is 0 or infinity
        double heuristic_info = (distance > 1e-9 && !std::isinf(distance)) ? 1.0 / distance : 0.0;

        double desirability = std::pow(pheromones[current_vertex][neighbor_vertex], alpha) *
                              std::pow(heuristic_info, beta);
        probabilities.push_back(desirability);
        total_desirability += desirability;
    }

    if (total_desirability < 1e-9) { // Avoid division by zero
        // Fallback: choose a random unvisited neighbor if all desirabilities are ~zero
        std::uniform_int_distribution<> dist(0, unvisited_neighbors.size() - 1);
        int chosen_neighbor_index = dist(rng);
        int next_vertex = unvisited_neighbors[chosen_neighbor_index];

        visited[next_vertex] = true;
        tour.push_back(next_vertex);
        current_vertex = next_vertex;
        return true;
    }

    // Normalize probabilities
    for (double& prob : probabilities) {
        prob /= total_desirability;
    }

    // Choose the next vertex based on probabilities
    std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    int chosen_neighbor_index = dist(rng);
    int next_vertex = unvisited_neighbors[chosen_neighbor_index];

    visited[next_vertex] = true;
    tour.push_back(next_vertex);
    current_vertex = next_vertex;

    return true;
}

void Ant::calculate_tour_cost(const Graph& graph) {
    tour_cost = 0.0;
    // A valid TSP tour must visit all vertices and return to the start, size num_vertices + 1
    if (tour.size() != num_vertices + 1 || tour.back() != tour.front()) {
        tour_cost = std::numeric_limits<double>::max(); // Invalid tour
        return;
    }

    for (size_t i = 0; i < tour.size() - 1; ++i) {
        int u = tour[i];
        int v = tour[i+1];
        Edge e; bool exists;
        boost::tie(e, exists) = boost::edge(u, v, graph);
        if (exists) {
            // Access edge weight directly using boost::get
            tour_cost += boost::get(boost::edge_weight, graph, e);
        } else {
            tour_cost = std::numeric_limits<double>::max(); // Path does not exist
            break;
        }
    }
}

void Ant::reset(int start_vertex) {
    tour.assign(1, start_vertex);
    tour_cost = 0.0;
    std::fill(visited.begin(), visited.end(), false);
    visited[start_vertex] = true;
    current_vertex = start_vertex;
}

// --- ACO_Solver Class Implementation ---

ACO_Solver::ACO_Solver(const Graph& g, int ants, int iterations, double evap_rate, double a, double b, double initial_phero) :
    graph(g),
    num_vertices(boost::num_vertices(g)),
    num_ants(ants),
    num_iterations(iterations),
    evaporation_rate(evap_rate),
    alpha(a),
    beta(b),
    initial_pheromone(initial_phero),
    pheromones(num_vertices, std::vector<double>(num_vertices)),
    best_tour_cost(std::numeric_limits<double>::max())
{
    // Initialize random number generator
    std::random_device rd;
    rng.seed(rd());

    initialize_pheromones();
}

void ACO_Solver::initialize_pheromones() {
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            // Only initialize pheromones on existing edges
            if (boost::edge(i, j, graph).second) {
                 pheromones[i][j] = initial_pheromone;
            } else {
                 pheromones[i][j] = 0.0; // No pheromones on non-existent edges
            }
        }
    }
}

void ACO_Solver::run() {
    std::vector<Ant> ants;
    // Create ants and place them at random starting vertices
    std::uniform_int_distribution<> start_node_dist(0, num_vertices - 1);
    for (int i = 0; i < num_ants; ++i) {
        ants.emplace_back(num_vertices, start_node_dist(rng));
    }

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        //std::cout << "Iteration " << iteration + 1 << "/" << num_iterations << std::endl; // Optional: Print progress

        // Construct tours for all ants
        construct_tours(ants);

        // Update pheromones
        update_pheromones(ants);

        // Reset ants for the next iteration
        for (auto& ant : ants) {
             ant.reset(start_node_dist(rng)); // Start from a potentially different random vertex
        }

        // Optional: Print best cost for this iteration
        // std::cout << "Best cost in iteration " << iteration + 1 << ": " << best_tour_cost << std::endl;
    }
}

void ACO_Solver::construct_tours(std::vector<Ant>& ants) {
    // Each ant constructs a full tour.
    // For TSP, ants need to visit all cities and return to the starting city.
    // This loop simulates steps until ants have built a potential full tour.
    // A full TSP tour has num_vertices + 1 elements (start -> ... -> last -> start).
    for (int step = 0; step < num_vertices; ++step) { // Ants make num_vertices steps to visit all cities
        for (size_t i = 0; i < ants.size(); ++i) {
             if (ants[i].tour.size() < num_vertices + 1) { // If tour is not yet complete (including return to start)
                 ants[i].move_to_next_vertex(graph, pheromones, alpha, beta, rng);
             }
        }
    }

    // After all ants have attempted to build a tour (potentially visiting all cities),
    // make the final move to return to the start vertex if possible.
    // This was already handled in move_to_next_vertex when unvisited_neighbors.empty().
    // Now, calculate the cost for all ants' constructed tours.
    for (auto& ant : ants) {
        ant.calculate_tour_cost(graph);

        // Update best tour if this one is better and valid
        if (ant.tour_cost < best_tour_cost) {
            best_tour_cost = ant.tour_cost;
            best_tour = ant.tour;
            // std::cout << "New best tour found with cost: " << best_tour_cost << std::endl; // Optional: Print when new best is found
        }
    }
}


void ACO_Solver::update_pheromones(const std::vector<Ant>& ants) {
    // Evaporation
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
             // Only evaporate on existing edges
             if (boost::edge(i, j, graph).second) {
                 pheromones[i][j] *= (1.0 - evaporation_rate);
             }
        }
    }

    // Deposition
    // Ants deposit pheromones based on the quality of their tour (lower cost = more pheromone)
    // Only deposit on edges used by successful ants (those with valid tours)
    for (const auto& ant : ants) {
        if (ant.tour_cost < std::numeric_limits<double>::max()) { // Check if the tour was valid
            // The amount of pheromone to deposit is inversely proportional to the tour cost
            double pheromone_deposit_amount = 1.0 / ant.tour_cost;

            // Deposit pheromones along the ant's tour
            deposit_pheromone(ant.tour, pheromone_deposit_amount);
        }
    }

    // Optional: Add pheromone to the best tour found so far (elitist strategy)
    if (best_tour_cost < std::numeric_limits<double>::max()) {
        // Deposit more pheromone on the global best tour
        double elitist_pheromone_deposit_amount = 1.0 / best_tour_cost;
        deposit_pheromone(best_tour, elitist_pheromone_deposit_amount);
    }

    // Optional: Implement pheromone limits (e.g., Max-Min Ant System) to avoid stagnation
    // This is more advanced and not included in this basic implementation but is an area for improvement.
}

double ACO_Solver::calculate_desirability(int u, int v, const Graph& graph, const std::vector<std::vector<double>>& pheromones, double alpha, double beta) const {
     Edge e; bool exists;
     boost::tie(e, exists) = boost::edge(u, v, graph);
     if (!exists) {
         return 0.0; // Cannot move if edge doesn't exist
     }

     // Access edge weight directly using boost::get
     double distance = boost::get(boost::edge_weight, graph, e);

    // Avoid division by zero or infinity distance
    double heuristic_info = (distance > 1e-9 && !std::isinf(distance)) ? 1.0 / distance : 0.0;

    // Desirability = (Pheromone^alpha) * (Heuristic^beta)
    return std::pow(pheromones[u][v], alpha) * std::pow(heuristic_info, beta);
}


void ACO_Solver::deposit_pheromone(const std::vector<int>& tour, double pheromone_amount) {
    // Deposit pheromone on each edge in the tour
    // The tour vector includes the return to the start for a complete TSP tour
    if (tour.size() > 1) {
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            int u = tour[i];
            int v = tour[i+1];
            // Check if the edge exists before depositing
             if (boost::edge(u, v, graph).second) {
                 pheromones[u][v] += pheromone_amount;
             }
        }
    }
}


int main()
{
    // --- Example Usage: Create a simple TSP graph ---
    // Let's create a graph representing a few cities
    int num_cities = 5;
    Graph my_graph(num_cities);

    // Add edges with weights (distances)
    // For a complete graph (common in basic TSP examples), add edges between all pairs of cities
    // Since it's TSP, distances are usually symmetric, so add both u->v and v->u edges.
    // If distances are not symmetric, keep directed edges as is.
    add_edge(0, 1, 10.0, my_graph); add_edge(1, 0, 10.0, my_graph);
    add_edge(0, 2, 15.0, my_graph); add_edge(2, 0, 15.0, my_graph);
    add_edge(0, 3, 20.0, my_graph); add_edge(3, 0, 20.0, my_graph);
    add_edge(0, 4, 25.0, my_graph); add_edge(4, 0, 25.0, my_graph);

    add_edge(1, 2, 35.0, my_graph); add_edge(2, 1, 35.0, my_graph);
    add_edge(1, 3, 25.0, my_graph); add_edge(3, 1, 25.0, my_graph);
    add_edge(1, 4, 30.0, my_graph); add_edge(4, 1, 30.0, my_graph);

    add_edge(2, 3, 30.0, my_graph); add_edge(3, 2, 30.0, my_graph);
    add_edge(2, 4, 20.0, my_graph); add_edge(4, 2, 20.0, my_graph);

    add_edge(3, 4, 10.0, my_graph); add_edge(4, 3, 10.0, my_graph);

    // --- ACO Parameters ---
    int num_ants = 5;
    int num_iterations = 200; // Increased iterations for better convergence
    double evaporation_rate = 0.5; // Rate at which pheromones evaporate
    double alpha = 1.0; // Pheromone factor (typical range 1-5)
    double beta = 2.0;  // Heuristic factor (distance) (typical range 2-5)
    double initial_pheromone = 1.0 / num_cities; // A common initial pheromone value heuristic

    // Create an ACO_Solver instance
    ACO_Solver solver(my_graph, num_ants, num_iterations, evaporation_rate, alpha, beta, initial_pheromone);

    // Run the algorithm
    solver.run();

    // Get the best tour and its cost
    std::vector<int> best_tour = solver.get_best_tour();
    double best_cost = solver.get_best_tour_cost();

    // Print results
    if (best_cost < std::numeric_limits<double>::max()) {
        std::cout << "\nBest tour found:" << std::endl;
        for (size_t i = 0; i < best_tour.size(); ++i) {
            std::cout << best_tour[i] << (i == best_tour.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
        std::cout << "Best tour cost: " << best_cost << std::endl;
    } else {
        std::cout << "\nNo valid tour found." << std::endl;
    }


    return 0;
}
