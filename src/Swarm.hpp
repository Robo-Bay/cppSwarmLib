#pragma once
#include "Params.hpp"
#include "SwarmUnit.hpp"
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <vector>
namespace swarm {

/**
 * @brief Link on swarm unit(agent), used in SwarmUnitContainer in Swarm class.
 *
 */
template <typename IUnitT = ISwarmUnit>
class SwarmUnitLink : public std::shared_ptr<IUnitT> {
  static_assert(std::is_base_of<ISwarmUnit, IUnitT>::value,
                "IUnitT must be derived from ISwarmUnit");

public:
  template <typename SwarmUnitT>
  SwarmUnitLink(SwarmUnitT *u) : std::shared_ptr<IUnitT>(u) {
    static_assert(std::is_base_of<ISwarmUnit, IUnitT>::value,
                  "SwarmUnitT must be derived from IUnitT");
  }
  using std::shared_ptr<IUnitT>::shared_ptr;
  ~SwarmUnitLink() {
    std::cout << std::shared_ptr<IUnitT>::use_count() << std::endl;
  }
};

/**
 * @brief Interface of swarm unit container, used in Swarm class
 *
 */
template <typename IUnitT = ISwarmUnit> class ISwarmUnitsContainer {
public:
  /**
   * @brief Construct a new ISwarmUnitsContainer object and try reserve size
   *
   * @param size
   */
  ISwarmUnitsContainer(std::size_t size) {}
  ISwarmUnitsContainer() {}
  virtual ~ISwarmUnitsContainer() = default;
  virtual void add_unit(SwarmUnitLink<IUnitT> unit) = 0;
  virtual void for_each(std::function<void(IUnitT &)>) const = 0;
  virtual void init() = 0;
  virtual void iter() = 0;
  virtual std::size_t size() const = 0;
  virtual std::size_t reserved_size() const = 0;
};

/**
 * @brief Реализация контейнера на основе std::vector
 */
template <typename IUnitT = ISwarmUnit>
class SwarmVectorContainer : public ISwarmUnitsContainer<IUnitT> {
  std::vector<SwarmUnitLink<IUnitT>> units_;

public:
  SwarmVectorContainer(std::size_t size)
      : units_(), ISwarmUnitsContainer<IUnitT>(size) {
    units_.reserve(size);
  }

  SwarmVectorContainer() : ISwarmUnitsContainer<IUnitT>() {}
  void add_unit(SwarmUnitLink<IUnitT> unit) override {
    units_.emplace_back(move(unit));
  }

  void for_each(std::function<void(IUnitT &)> action) const override {
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
  std::size_t reserved_size() const override { return units_.capacity(); };
};

/**
 * @brief Реализация контейнера на основе std::unordered_set
 */
template <typename IUnitT = ISwarmUnit>
class SwarmUnorderedSetContainer : public ISwarmUnitsContainer<IUnitT> {
  struct Hash {
    size_t operator()(const SwarmUnitLink<IUnitT> &ptr) const {
      return std::hash<IUnitT *>()(ptr.get());
    }
  };

  struct Equal {
    bool operator()(const SwarmUnitLink<IUnitT> &a,
                    const SwarmUnitLink<IUnitT> &b) const {
      return a.get() == b.get();
    }
  };

  std::unordered_set<SwarmUnitLink<IUnitT>, Hash, Equal> units_;

public:
  SwarmUnorderedSetContainer(std::size_t size)
      : units_(size), ISwarmUnitsContainer<IUnitT>(size) {}
  SwarmUnorderedSetContainer() : ISwarmUnitsContainer<IUnitT>() {}
  void add_unit(SwarmUnitLink<IUnitT> unit) override {
    units_.insert(std::move(unit));
  }

  void for_each(std::function<void(IUnitT &)> action) const override {
    for (const auto &unit : units_) {
      if (unit)
        action(*unit);
    }
  }
  std::size_t size() const override { return units_.size(); }
  std::size_t reserved_size() const override { return units_.capacity(); }
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
template <template <class> class SwarmUnitsContainerT, class SwarmParamsT,
          class IUnitT = ISwarmUnit>
class Swarm : public Parameterizable<SwarmParamsT> {
  static_assert(std::is_base_of<IParams, SwarmParamsT>::value,
                "UnitParams must be derived from IParams");
  static_assert(
      std::is_base_of<ISwarmUnitsContainer<IUnitT>,
                      SwarmUnitsContainerT<IUnitT>>::value,
      "SwarmUnitsContainerT must me derived from ISwarmUnitsContainer");
  static_assert(std::is_base_of<ISwarmUnit, IUnitT>::value,
                "IUnitT must be derived from ISwarmUnit");

protected:
  SwarmUnitsContainerT<IUnitT> _Units;
  SwarmParamsT _Params;

public:
  Swarm(std::size_t sz = 0) : _Units(sz) {}
  Swarm(const SwarmParamsT &params, std::size_t sz = 0)
      : _Params(params), _Units(sz) {}
  virtual void init() { _Units.init(); }
  template <typename T> void init(bool create_reserve_units) {
    static_assert(std::is_base_of<IUnitT, T>::value,
                  "T must be derived of IUnitT");
    if (create_reserve_units) {
      for (std::size_t i = 0; i < _Units.reserved_size() - _Units.size(); ++i) {
        _Units.add_unit(SwarmUnitLink<IUnitT>(new T()));
      }
    }
    init();
  }
  virtual void iter() { _Units.iter(); }
  void for_each(std::function<void(IUnitT &)> action) {
    _Units.for_each(action);
  }
  virtual ~Swarm() = default;
};
} // namespace swarm