#pragma once

#include "../SwarmUnit.hpp"
#include <cstdint>
#include <type_traits>
namespace swarm {
static constexpr uint32_t MaximumTaskLvl = 256;

template <typename SwarmUnitT> class IBaseTask {
  static_assert(
      std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
                                     typename SwarmUnitT::CommunicationCT,
                                     typename SwarmUnitT::TaskManagerCT,
                                     typename SwarmUnitT::ExecutorCT>,
                      SwarmUnitT>::value,
      "SwarnUnitT must be derived of BasicSwarmUnit");
  SwarmUnitT *_unit;
  const ITaskParams &_params;

public:
  IBaseTask(SwarmUnitT *, const ITaskParams &);
};

/**
 * @brief  Inteface of Task for drones.
 *
 * @tparam Lvl of task. Hard tasks has high lvl simple task has low lvl
 * @tparam SwarmUnitT
 * @tparam TaskParamsT
 */
template <uint32_t Lvl, typename SwarmUnitT, typename TaskParamsT>
class ITask : public IBaseTask<SwarmUnitT> {
  static_assert(Lvl < MaximumTaskLvl, "Lvl of task must be < MaximumTaskLvl");
  static_assert(std::is_base_of<TaskParamsT, ITaskParams>::value,
                "TaskParamsT must be derived by ITaskParams");
};

template <typename SwarmUnitT, typename TaskParamsT>
class ITask<0, SwarmUnitT, TaskParamsT> {};
template <typename SwarmUnitT, typename TaskParamsT>
class ITask<MaximumTaskLvl - 1, SwarmUnitT, TaskParamsT> {};

template <typename SwarmUnitT, typename TaskParamsT>
using MinimumTask = class ITask<0, SwarmUnitT, TaskParamsT>;

template <typename SwarmUnitT, typename TaskParamsT>
using MaximumTask = ITask<MaximumTaskLvl - 1, SwarmUnitT, TaskParamsT>;

template <typename SwarmUnitT>
using EmptyTask = MinimumTask<SwarmUnitT, EmptyTaskParams>;
} // namespace swarm