#pragma once
#include "../Params.hpp"
#include <algorithm>
#include <cstdint>
#include <type_traits>
namespace swarm {
static constexpr uint32_t MaximumTaskLvl = 256;

/**
 * @brief Interface for the task. Task have pointer on SwarmUnit which owns this
 * task, the const parameters that we set the task with and lvl of task. Lvl is
 * the the complexity of the task
 *
 * @tparam SwarmUnitT
 */
template <class SwarmUnitT> class IBaseTask {
  static_assert(std::is_base_of<typename SwarmUnitT::BasicSwarmUnit::UnitT,
                                SwarmUnitT>::value,
                "SwarmUnitT must be derived from BasicSwarmUnit");
  SwarmUnitT *_unit;
  const ITaskParams &_params;
  bool is_full_decomposed;
  uint32_t _Lvl;

public:
  /**
   * @brief Construct a new IBaseTask object
   *
   * @param lvl - complexity of the task, <= MaximumTaskLvl
   * @param u - pointer to unit
   * @param p - the parameters that task is set
   */
  IBaseTask(uint32_t lvl, SwarmUnitT *u, const ITaskParams &p)
      : _Lvl(std::min(lvl, MaximumTaskLvl - 1)), _unit(u), _params(p) {}
  constexpr uint32_t get_lvl() const { return _Lvl; }
  virtual ~IBaseTask() = default;
};

// TODO Decision tree(on TaskParamsT) for decompose task

template <uint32_t Lvl, typename SwarmUnitT, typename TaskParamsT>
class ITask : public IBaseTask<SwarmUnitT> {
  static_assert(std::is_base_of<ITaskParams, TaskParamsT>::value,
                "TaskParamsT must be derived by ITaskParams");
  static_assert(Lvl < MaximumTaskLvl, "Lvl of task must be < MaximumTaskLvl");

public:
  ITask(SwarmUnitT *u, const TaskParamsT &params)
      : IBaseTask<SwarmUnitT>(Lvl, u, params) {}
    
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