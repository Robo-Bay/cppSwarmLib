#pragma once
#include "../Tasks/ITask.hpp"
#include "IUnitComponent.hpp"
#include <deque>
#include <memory>

namespace swarm {
/**
 * @brief Interface of the task manager swarm unit component. This module get,
 * decompose and manage the tasks from swarm or other units.
 *
 */
template <class SwarmUnitT, class ParamsT>
class ITaskManagerUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
  using TaskT = std::shared_ptr<IBaseTask<SwarmUnitT>>;
  std::deque<TaskT> _tasks_deque;
  TaskT current_task;

public:
  virtual ~ITaskManagerUnitC() = 0;
  void add_task(TaskT, bool add_in_back = false);
  TaskT decompose_task(TaskT) const;
};
template <typename SwarmUnitT, typename ParamsT = EmptyParams>
class EmptyTaskManagerC : public EmptyUnitComponent<SwarmUnitT, ParamsT> {
public:
  ~EmptyTaskManagerC() final = default;
};
} // namespace swarm