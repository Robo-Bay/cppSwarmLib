#pragma once
#include "Params.hpp"
#include "SwarmUnit.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <vector>
namespace swarm {

/**
 * @brief Link on swarm unit(agent), used in SwarmUnitContainer in Swarm class.
 *
 */
class SwarmUnitLink : public std::shared_ptr<ISwarmUnit> {
public:
  template <typename SwarmUnitT>
  SwarmUnitLink(SwarmUnitT *u) : std::shared_ptr<ISwarmUnit>(u) {
    static_assert(std::is_base_of<ISwarmUnit, SwarmUnitT>::value,
                  "SwarmUnitT must be derived from ISwarmUnit");
  }
  using std::shared_ptr<ISwarmUnit>::shared_ptr;
};

/**
 * @brief Interface of swarm unit container, used in Swarm class
 *
 */
class ISwarmUnitsContainer {
public:
  /**
   * @brief Construct a new ISwarmUnitsContainer object and try reserve size
   *
   * @param size
   */
  ISwarmUnitsContainer(std::size_t size) {}
  ISwarmUnitsContainer() {}
  virtual ~ISwarmUnitsContainer() = default;
  virtual void add_unit(SwarmUnitLink unit) = 0;
  virtual void for_each(std::function<void(ISwarmUnit &)>) const = 0;
  virtual void init() = 0;
  virtual void iter() = 0;
  virtual std::size_t size() const = 0;
};

/**
 * @brief Реализация контейнера на основе std::vector
 */
class SwarmVectorContainer : public ISwarmUnitsContainer {
  std::vector<SwarmUnitLink> units_;

public:
  SwarmVectorContainer(std::size_t size)
      : units_(), ISwarmUnitsContainer(size) {
    units_.reserve(size);
  }

  SwarmVectorContainer() : ISwarmUnitsContainer() {}
  void add_unit(SwarmUnitLink unit) override {
    units_.push_back(std::move(unit));
  }

  void for_each(std::function<void(ISwarmUnit &)> action) const override {
    for (const auto &unit : units_) {
      if (unit)
        action(*unit);
    }
  }
  void init() override {
    for (const auto &unit : units_) {
      unit->init();
    }
  }
  void iter() override {
    for (const auto &unit : units_) {
      unit->iter();
    }
  }
  std::size_t size() const override { return units_.size(); }
};

/**
 * @brief Реализация контейнера на основе std::unordered_set
 */
class SwarmUnorderedSetContainer : public ISwarmUnitsContainer {
  struct Hash {
    size_t operator()(const SwarmUnitLink &ptr) const {
      return std::hash<ISwarmUnit *>()(ptr.get());
    }
  };

  struct Equal {
    bool operator()(const SwarmUnitLink &a, const SwarmUnitLink &b) const {
      return a.get() == b.get();
    }
  };

  std::unordered_set<SwarmUnitLink, Hash, Equal> units_;

public:
  SwarmUnorderedSetContainer(std::size_t size)
      : units_(size), ISwarmUnitsContainer(size) {}
  SwarmUnorderedSetContainer() : ISwarmUnitsContainer() {}
  void add_unit(SwarmUnitLink unit) override { units_.insert(std::move(unit)); }

  void for_each(std::function<void(ISwarmUnit &)> action) const override {
    for (const auto &unit : units_) {
      if (unit)
        action(*unit);
    }
  }
  std::size_t size() const override { return units_.size(); }
  void init() override {
    for (const auto &unit : units_) {
      unit->init();
    }
  }
  void iter() override {
    for (const auto &unit : units_) {
      unit->iter();
    }
  }
};

/**
 * @brief The main Swarm class for the multiagent and swarm systems. The shell
 * above SwarmUnitContainer
 *
 * @tparam SwarmUnitsContainerT container what contains the members of swarm
 * @tparam SwarmParamsT parameters of the swarm
 */
template <class SwarmUnitsContainerT, class SwarmParamsT>
class Swarm : public Parameterizable<SwarmParamsT> {
  static_assert(std::is_base_of<IParams, SwarmParamsT>::value,
                "UnitParams must be derived from IParams");
  static_assert(
      std::is_base_of<ISwarmUnitsContainer, SwarmUnitsContainerT>::value,
      "SwarmUnitsContainerT must me derived from ISwarmUnitsContainer");

  SwarmUnitsContainerT _Units;
  SwarmParamsT _Params;

public:
  Swarm() = default;
  Swarm(const SwarmParamsT &params) : _Params(params) {}
  virtual void init() { _Units.init(); }
  virtual void iter() { _Units().iter(); }
  virtual ~Swarm() = default;
};
} // namespace swarm