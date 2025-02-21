#pragma once
#include "SwarmUnit.hpp"
#include <iterator>
#include <memory>
#include <type_traits>
namespace swarm {

/**
 * @brief Link on swarm unit(agent), used in SwarmUnitContainer in Swarm class.
 *
 */
class SwarmUnitLink : public std::unique_ptr<ISwarmUnit> {};

/**
 * @brief Interface of swarm unit container, used in Swarm class
 *
 */
class ISwarmUnitsContainer {
public:
  using Iterator = std::forward_iterator_tag;
  using ConstIterator = std::forward_iterator_tag;
  ISwarmUnitsContainer() = default;
  virtual SwarmUnitLink add_unit(SwarmUnitLink) = 0;
  virtual Iterator begin() = 0;
  virtual Iterator end() = 0;
  virtual ConstIterator begin() const = 0;
  virtual ConstIterator end() const = 0;
  virtual ~ISwarmUnitsContainer() = default;
};
/**
 * @brief The main Swarm class for the multiagent and swarm systems. The shell above SwarmUnitContainer
 * 
 * @tparam SwarmUnitsContainerT container what contains the members of swarm
 * @tparam SwarmParamsT parameters of the swarm
 */
template <class SwarmUnitsContainerT, class SwarmParamsT> class Swarm {
  static_assert(std::is_base_of<IParams, SwarmParamsT>::value,
                "UnitParams must be derived from IParams");
  static_assert(
      std::is_base_of<SwarmUnitsContainerT, ISwarmUnitsContainer>::value,
      "SwarmUnitsContainerT must me derived from ISwarmUnitsContainer");

  SwarmUnitsContainerT _Units;
  SwarmParamsT _Params;

public:
  Swarm() = default;
  Swarm(const SwarmParamsT &params) : _Params(params) {}
  virtual void init() {
    for (auto u : _Units) {
      u->init();
    }
  }
  virtual void iter() {
    for (auto u : _Units) {
      u->iter();
    }
  }
  virtual ~Swarm() = default;
};
} // namespace swarm