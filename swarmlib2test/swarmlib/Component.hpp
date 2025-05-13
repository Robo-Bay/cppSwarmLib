// Component.hpp
#pragma once
#include <concepts> // C++20 <concepts> | C++17: Remove or replace with SFINAE/static_assert
#include <type_traits> // For static_asserts, std::is_base_of

namespace swarmlib {

// Forward declaration Agent needs Derived + Components now
template <typename DerivedAgent, typename... ComponentsT> class Agent;
class IAgent; // Base interface remains the same

class IComponent {
public:
  virtual ~IComponent() = default;
  virtual void init() = 0;
  virtual void iter() = 0;
};

// Component template now depends on the final Agent type (e.g., DroneAgent)
template <typename OwnerAgentT> // Renamed AgentT -> OwnerAgentT for clarity
class Component : public IComponent {
  // C++17 check example:
  // static_assert(std::is_base_of<IAgent, OwnerAgentT>::value, "OwnerAgentT
  // must derive from IAgent");

protected:
  OwnerAgentT &_agent;

public:
  // Constructor now expects the final derived agent type
  explicit Component(OwnerAgentT &agent) : _agent(agent) {}
  ~Component() override = default;

  Component(const Component &) = delete;
  Component &operator=(const Component &) = delete;
  Component(Component &&) = default;
  Component &operator=(Component &&) = default;

  void init() override = 0;
  void iter() override = 0;
};

} // namespace swarmlib