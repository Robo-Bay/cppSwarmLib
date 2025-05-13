// main.cpp
#include "Swarm.hpp"
#include <cmath> // For potential use in components
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

// --- Define Concrete Components ---
class DroneAgent; // Forward declaration

// Component definition requires the final agent type (DroneAgent)
class MovementComponent : public swarmlib::Component<DroneAgent> {
  double x = 0.0, y = 0.0, z = 0.0;

public:
  // Constructor now implicitly expects DroneAgent& via inheritance
  using swarmlib::Component<DroneAgent>::Component;

  void init() override {
    std::cout << "  Movement Init\n";
    x = rand() % 100;
    y = rand() % 100;
    z = rand() % 10;
  }
  void iter() override {
    x += (rand() % 3 - 1) * 0.5;
    y += (rand() % 3 - 1) * 0.5;
  }
  void print_position() const {
    std::cout << "Pos:(" << round(x) << "," << round(y) << "," << round(z)
              << ")";
  }
  double get_x() const { return x; }
  double get_y() const { return y; }
};

class CommunicationComponent : public swarmlib::Component<DroneAgent> {
  std::string agent_id;

public:
  using swarmlib::Component<DroneAgent>::Component; // Expects DroneAgent&

  void init() override {
    agent_id = "Drone_" + std::to_string(rand() % 1000);
    std::cout << "  Comm Init [" << agent_id << "]\n";
  }
  void iter() override { /* ... */ }
  const std::string &get_id() const { return agent_id; }
};

class TaskComponent : public swarmlib::Component<DroneAgent> {
  std::string current_task = "Idle";

public:
  using swarmlib::Component<DroneAgent>::Component; // Expects DroneAgent&

  void init() override {
    current_task = (rand() % 2 == 0) ? "Scouting" : "MovingToTarget";
    std::cout << "  Task Init\n";
  }
  void iter() override { /* ... */ }
  const std::string &get_task() const { return current_task; }
};

// --- Define Concrete Agent ---
// Inherits from Agent<DerivedAgent, Components...>
class DroneAgent
    : public swarmlib::Agent<DroneAgent, // Pass self-type (CRTP)
                             MovementComponent, CommunicationComponent,
                             TaskComponent> {
  // Define Alias for easier access to base type if needed
  using BaseAgent = swarmlib::Agent<DroneAgent, MovementComponent,
                                    CommunicationComponent, TaskComponent>;

public:
  // MUST define a constructor that calls the protected base constructor
  DroneAgent()
      : BaseAgent(*this) // Pass derived 'this' to the base Agent constructor
  {
    std::cout
        << "DroneAgent constructed.\n"; // Optional: Check construction order
  }

  // Agent-specific methods
  void report_status() const {
    // Access components via get_component() (inherited)
    if constexpr (has_component<CommunicationComponent>() &&
                  has_component<MovementComponent>() &&
                  has_component<TaskComponent>()) {
      const auto &comm = get_component<CommunicationComponent>();
      const auto &move = get_component<MovementComponent>();
      const auto &task = get_component<TaskComponent>();
      std::cout << "[" << comm.get_id() << "] Status: Task='" << task.get_task()
                << "', ";
      move.print_position();
      std::cout << std::endl;
    } else {
      std::cout << "Agent missing required components for status report.\n";
    }
  }
};

// --- Main Simulation ---
int main() {
  srand(time(0));

  swarmlib::Swarm<swarmlib::VectorAgentsContainer> drone_swarm(10);

  std::cout << "Populating swarm...\n";
  // This now works because DroneAgent() constructor calls the correct base
  // constructor
  drone_swarm.populate<DroneAgent>(5);
  std::cout << "Population complete. Agent count: " << drone_swarm.agent_count()
            << "\n\n";

  std::cout << "Initializing swarm (potentially parallel)...\n";
  drone_swarm.init(); // Calls init() on all agents -> components
  std::cout << "Initialization complete.\n\n";

  const int num_iterations = 5;
  std::cout << "Running simulation for " << num_iterations
            << " iterations (potentially parallel)...\n";
  for (int i = 0; i < num_iterations; ++i) {
    std::cout << "--- Iteration " << i + 1 << " ---\n";
    drone_swarm.iter(); // Calls iter() on all agents -> components

    if ((i + 1) % 2 == 0) {
      std::cout << "  Status Report (sequential):\n";
      drone_swarm.for_each_agent([](swarmlib::IAgent &agent) {
        // dynamic_cast still works as DroneAgent derives from IAgent (via
        // Agent<...>)
        if (auto *drone = dynamic_cast<DroneAgent *>(&agent)) {
          drone->report_status();
        }
      });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  std::cout << "Simulation finished.\n";

  return 0;
}