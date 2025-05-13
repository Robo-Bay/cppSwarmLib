// Swarm.hpp
#pragma once

#include "Agent.hpp"
#include <algorithm> // For std::for_each
#include <concepts> // C++20 <concepts> | C++17: Remove 'requires' clauses or replace with SFINAE/static_assert
#include <execution> // For std::execution::par (C++17)
#include <functional>
#include <memory>
#include <type_traits> // std::is_base_of, std::derived_from alternative
#include <vector>

namespace swarmlib {

/**
 * @brief Link on swarm unit(agent), used in Agent containers.
 */
template <typename AgentInterfaceT = IAgent>
class AgentLink : public std::shared_ptr<AgentInterfaceT> {
  // C++17 check: std::is_base_of<IAgent, AgentInterfaceT>::value
  static_assert(std::is_base_of_v<IAgent, AgentInterfaceT>,
                "AgentInterfaceT must be derived from IAgent");

public:
  // C++20 Constructor with requires clause:
  template <typename ConcreteAgentT>
    requires std::derived_from<ConcreteAgentT, AgentInterfaceT>
  AgentLink(ConcreteAgentT *agent_ptr)
      : std::shared_ptr<AgentInterfaceT>(agent_ptr) {}
  // C++17 Alternative using SFINAE:
  // template <typename ConcreteAgentT,
  //           typename = std::enable_if_t<std::is_base_of<AgentInterfaceT,
  //           ConcreteAgentT>::value>>
  // AgentLink(ConcreteAgentT* agent_ptr)
  //     : std::shared_ptr<AgentInterfaceT>(agent_ptr) {}
  // C++17 Alternative using static_assert (simpler, error at instantiation):
  // template <typename ConcreteAgentT>
  // AgentLink(ConcreteAgentT* agent_ptr)
  //     : std::shared_ptr<AgentInterfaceT>(agent_ptr) {
  //         static_assert(std::is_base_of<AgentInterfaceT,
  //         ConcreteAgentT>::value, "ConcreteAgentT must derive from
  //         AgentInterfaceT");
  //     }

  using std::shared_ptr<AgentInterfaceT>::shared_ptr; // Inherit other
                                                      // constructors
};

/**
 * @brief Interface for agent containers within a Swarm.
 */
template <typename AgentInterfaceT = IAgent> class IAgentsContainer {
public:
  IAgentsContainer() = default;
  explicit IAgentsContainer(std::size_t size_hint) {}
  virtual ~IAgentsContainer() = default;

  IAgentsContainer(const IAgentsContainer &) = delete;
  IAgentsContainer &operator=(const IAgentsContainer &) = delete;
  IAgentsContainer(IAgentsContainer &&) = delete;
  IAgentsContainer &operator=(IAgentsContainer &&) = delete;

  virtual void add_agent(AgentLink<AgentInterfaceT> agent) = 0;
  // Apply action potentially in parallel - implementation detail
  virtual void
  for_each(std::function<void(AgentInterfaceT &)> action) const = 0;
  // Init agents potentially in parallel
  virtual void init() = 0;
  // Iterate agents potentially in parallel
  virtual void iter() = 0;
  virtual std::size_t size() const = 0;
  virtual std::size_t capacity() const = 0;
};

/**
 * @brief A default agent container implementation using std::vector.
 * Supports parallel iteration.
 */
template <typename AgentInterfaceT = IAgent>
class VectorAgentsContainer : public IAgentsContainer<AgentInterfaceT> {
  std::vector<AgentLink<AgentInterfaceT>> _agents;

public:
  explicit VectorAgentsContainer(std::size_t size_hint = 0)
      : IAgentsContainer<AgentInterfaceT>(size_hint) {
    if (size_hint > 0) {
      _agents.reserve(size_hint);
    }
  }

  void add_agent(AgentLink<AgentInterfaceT> agent) override {
    _agents.emplace_back(std::move(agent));
  }

  // Applies action sequentially (const method)
  void for_each(std::function<void(AgentInterfaceT &)> action) const override {
    std::for_each(std::execution::seq, _agents.begin(),
                  _agents.end(), // Use seq for const methods
                  [&action](const auto &agent_link) {
                    if (agent_link) {
                      action(*agent_link);
                    }
                  });
  }

  // Initializes agents in parallel
  void init() override {
    // Warning: Ensure Agent::init() is thread-safe if it accesses shared
    // resources!
    std::for_each(std::execution::par, _agents.begin(),
                  _agents.end(), // C++17 parallel execution
                  [](const auto &agent_link) {
                    if (agent_link)
                      agent_link->init();
                  });
  }

  // Iterates agents in parallel
  void iter() override {
    // Warning: Ensure Agent::iter() is thread-safe if it accesses shared
    // resources or if agents modify each other's state concurrently!
    std::for_each(std::execution::par, _agents.begin(),
                  _agents.end(), // C++17 parallel execution
                  [](const auto &agent_link) {
                    if (agent_link)
                      agent_link->iter();
                  });
  }

  std::size_t size() const override { return _agents.size(); }
  std::size_t capacity() const override { return _agents.capacity(); }
};
// Swarm class remains largely the same structurally
template <template <class> class ContainerT, typename AgentInterfaceT = IAgent>
  requires std::derived_from<ContainerT<AgentInterfaceT>,
                             IAgentsContainer<AgentInterfaceT>> &&
           std::is_base_of_v<IAgent, AgentInterfaceT>
// C++17 Alternative...
class Swarm {
  // C++17 static_assert checks...
protected:
  ContainerT<AgentInterfaceT> _agents;

public:
  explicit Swarm(std::size_t capacity_hint = 0) : _agents(capacity_hint) {}
  virtual ~Swarm() = default;
  Swarm(const Swarm &) = delete;
  Swarm &operator=(const Swarm &) = delete;
  Swarm(Swarm &&) = delete;
  Swarm &operator=(Swarm &&) = delete;

  void add_agent(AgentLink<AgentInterfaceT> agent) {
    _agents.add_agent(std::move(agent));
  }

  template <typename ConcreteAgentT>
    requires std::derived_from<ConcreteAgentT, AgentInterfaceT>
  // C++17 Alternative...
  void populate(std::size_t count) {
    // C++17 static_assert Alternative...
    for (std::size_t i = 0; i < count; ++i) {
      // Creating the agent requires its default constructor to work
      _agents.add_agent(AgentLink<AgentInterfaceT>(new ConcreteAgentT()));
    }
  }

  virtual void init() { _agents.init(); }
  virtual void iter() { _agents.iter(); }
  void for_each_agent(std::function<void(AgentInterfaceT &)> action) const {
    _agents.for_each(action);
  }
  std::size_t agent_count() const { return _agents.size(); }
  std::size_t capacity() const { return _agents.capacity(); }
  const ContainerT<AgentInterfaceT> &get_container() const { return _agents; }
  ContainerT<AgentInterfaceT> &get_container() { return _agents; }
};
} // namespace swarmlib