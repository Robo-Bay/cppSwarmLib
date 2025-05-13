// Agent.hpp
#pragma once

#include "Component.hpp"
#include <concepts> // C++20 <concepts> | C++17: Remove or replace
#include <stdexcept>
#include <tuple>
#include <type_traits> // For std::is_base_of, std::is_same, etc.
#include <utility>

namespace swarmlib {

class IAgent {
public:
  virtual ~IAgent() = default;
  virtual void init() = 0;
  virtual void iter() = 0;
};

// Helper concept now checks Component<DerivedAgent> and constructibility with
// DerivedAgent& C++20 Concept:
template <typename CompT, typename DerivedAgent>
concept IsComponentFor =
    std::derived_from<CompT, IComponent> &&
    std::derived_from<CompT,
                      Component<DerivedAgent>> &&    // Check it derives from
                                                     // Component<DerivedAgent>
    requires(DerivedAgent &agent) { CompT(agent); }; // Check constructibility
// C++17 Alternative:
// template<typename CompT, typename DerivedAgent>
// struct CheckIsComponentFor {
//     static constexpr bool value = std::is_base_of<IComponent, CompT>::value
//     &&
//                                   std::is_base_of<Component<DerivedAgent>,
//                                   CompT>::value && // Check base
//                                   std::is_constructible<CompT,
//                                   DerivedAgent&>::value; // Check constructor
// };

// Agent uses CRTP: knows the DerivedAgent type
template <typename DerivedAgent, typename... ComponentsT>
class Agent : public IAgent {
  // Ensure DerivedAgent actually derives from this Agent specialization
  //   static_assert(
  //       std::derived_from<DerivedAgent, Agent<DerivedAgent, ComponentsT...>>
  //       ==
  //           true,
  //       "DerivedAgent must inherit from Agent<DerivedAgent,
  //       ComponentsT...>");
  // Ensure all ComponentsT derive from Component<DerivedAgent> and are
  // constructible C++20 check:
  static_assert((IsComponentFor<ComponentsT, DerivedAgent> && ...),
                "All ComponentsT must be valid components for DerivedAgent");
  // C++17 check:
  // static_assert((CheckIsComponentFor<ComponentsT, DerivedAgent>::value &&
  // ...),
  //               "All ComponentsT must be valid components for DerivedAgent");

private:
  std::tuple<ComponentsT...> _components;

  // Helper constructor needs access to the derived object to pass to components

protected:
  // Protected constructor called by DerivedAgent. Passes the derived 'this'.
  // We need a way to get the derived this pointer during base construction.
  // A common CRTP pattern is to pass it explicitly, or use a post-construction
  // init. Let's refine the default constructor approach.

  // Default constructor will now need access to the derived 'this'.
  // This makes default construction tricky. A common pattern is a two-phase
  // init or factory. Let's try passing derived 'this' pointer in the
  // constructor call chain.

  Agent(DerivedAgent &derived_this)
      // Pass the derived reference to each component constructor
      : _components(ComponentsT(derived_this)...) {}

public:
  // Derived class MUST call the protected Agent constructor
  // Example: DroneAgent() : BaseAgent(this) {} where BaseAgent is
  // Agent<DroneAgent, ...>

  // Making default constructor public might be misleading as it needs derived
  // this. Let's remove the public default constructor and force derived classes
  // to call the protected one.
  Agent() = delete; // Prevent default construction without derived ptr

  ~Agent() override = default;

  Agent(const Agent &) = delete;
  Agent &operator=(const Agent &) = delete;
  Agent(Agent &&) = delete;
  Agent &operator=(Agent &&) = delete;

  void init() override {
    std::apply([](auto &...comp) { (comp.init(), ...); }, _components);
  }

  void iter() override {
    std::apply([](auto &...comp) { (comp.iter(), ...); }, _components);
  }

  // get_component and has_component remain the same logically,
  // but are now part of Agent<DerivedAgent, ComponentsT...>

  template <typename CompT> CompT &get_component() {
    // C++17: std::is_same<CompT, ComponentsT>::value
    static_assert((std::is_same_v<CompT, ComponentsT> || ...),
                  "Component type not found in this Agent");
    return std::get<CompT>(_components);
  }

  template <typename CompT> const CompT &get_component() const {
    // C++17: std::is_same<CompT, ComponentsT>::value
    static_assert((std::is_same_v<CompT, ComponentsT> || ...),
                  "Component type not found in this Agent");
    return std::get<CompT>(_components);
  }

  template <typename CompT> static constexpr bool has_component() {
    // C++17: std::is_same<CompT, ComponentsT>::value
    return (std::is_same_v<CompT, ComponentsT> || ...);
  }
};

} // namespace swarmlib
