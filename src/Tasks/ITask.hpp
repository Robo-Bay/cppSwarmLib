#pragma once

#include "../Params.hpp"
#include "../SwarmUnit.hpp"
#include <cstdint>
#include <type_traits>
namespace swarm {
static constexpr uint32_t MaximumTaskLvl = 256;
/**
 * @brief Parameters for the task. They are set once when a task is set.
 *
 */
template <typename SwarmUnitT> class IBaseTask {
  static_assert(std::is_base_of<typename SwarmUnitT::BasicSwarmUnit::UnitT,
                                SwarmUnitT>::value,
                "SwarmUnitT must be derived from BasicSwarmUnit");
  SwarmUnitT *_unit;
  const ITaskParams &_params;
  bool is_full_decomposed;

public:
  using TaskManager = typename SwarmUnitT::BasicSwarmUnit::_TaskManagerT;
  IBaseTask(SwarmUnitT *u, const ITaskParams &p) : _unit(u), _params(p) {}
  virtual constexpr uint32_t get_lvl() const = 0;
  virtual ~IBaseTask() = default;
  friend TaskManager;
};

template <uint32_t Lvl, typename SwarmUnitT, typename TaskParamsT>
class ITask : public IBaseTask<SwarmUnitT> {
  static_assert(std::is_base_of<ITaskParams, TaskParamsT>::value,
                "TaskParamsT must be derived by ITaskParams");
  static_assert(Lvl < MaximumTaskLvl, "Lvl of task must be < MaximumTaskLvl");

public:
  constexpr uint32_t get_lvl() const final { return Lvl; };
  virtual ~ITask() = default;
};

template <typename SwarmUnitT, typename TaskParamsT>
class ITask<0, SwarmUnitT, TaskParamsT> {};
template <typename SwarmUnitT, typename TaskParamsT>
class ITask<MaximumTaskLvl - 1, SwarmUnitT, TaskParamsT> {};

template <typename SwarmUnitT, typename TaskParamsT>
using MinimumTask = class ITask<0, SwarmUnitT, TaskParamsT>;

template <typename SwarmUnitT, typename TaskParamsT>
using MaximumTask = ITask<MaximumTaskLvl - 1, SwarmUnitT, TaskParamsT>;
} // namespace swarm