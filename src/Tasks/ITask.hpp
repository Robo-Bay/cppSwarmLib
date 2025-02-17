#pragma once

#include "../SwarmUnit.hpp"
#include <cstdint>
#include <type_traits>
namespace swarm {
static constexpr uint32_t MaximumTaskLvl = 256;
/**
 * @brief Parameters for the task. They are set once when a task is set.
 *
 */
class ITaskParams {};
template <typename SwarmUnitT, typename TaskParamsT> class IBaseTask {
  static_assert(
      std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
                                     typename SwarmUnitT::CommunicationCT,
                                     typename SwarmUnitT::TaskManagerCT,
                                     typename SwarmUnitT::ExecutorCT>,
                      SwarmUnitT>::value,
      "SwarnUnitT must be derived of BasicSwarmUnit");
  static_assert(std::is_base_of<TaskParamsT, ITaskParams>::value,
                "TaskParamsT must be derived by ITaskParams");
  SwarmUnitT *_unit;
  const TaskParamsT &_params;

public:
  IBaseTask(SwarmUnitT *, const TaskParamsT &);
};

template <uint32_t Lvl, typename SwarmUnitT, typename TaskParamsT>
class ITask : public IBaseTask<SwarmUnitT, TaskParamsT> {
  static_assert(Lvl < MaximumTaskLvl, "Lvl of task must be < MaximumTaskLvl");
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