#pragma once
#include "../Tasks/ITask.hpp"
#include "IUnitComponent.hpp"
#include <deque>

namespace swarm {
template <class ParamsT, class SwarmUnitT> class IUnitComponent;
/**
 * @brief Interface of the task manager swarm unit component. This module get,
 * decompose and manage the tasks from swarm or other units.
 *
 */
template <class ParamsT, class SwarmUnitT>
class ITaskManagerUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  ITaskManagerUnitC(SwarmUnitT *u) : IUnitComponent<ParamsT, SwarmUnitT>(u) {}
  virtual ~ITaskManagerUnitC() = default;
};

template <class SwarmUnitT>
class EmptyTaskManagerC : public ITaskManagerUnitC<EmptyParams, SwarmUnitT> {
public:
  EmptyTaskManagerC(SwarmUnitT *u)
      : ITaskManagerUnitC<EmptyParams, SwarmUnitT>(u) {}
  ~EmptyTaskManagerC() = default;
  void init() final {};
  void iter() final {};
};
} // namespace swarm