#pragma once
#include "../Tasks/ITask.hpp"
#include "IUnitComponent.hpp"
#include <cstdint>
#include <deque>
#include <memory>
#include <utility>

namespace swarm {

template <class SwarmUnitT>
class PtrTask : public std::shared_ptr<IBaseTask<SwarmUnitT>> {};
/**
 * @brief TaskDeque. Task on front exucute first, on back last
 *
 */
template <class SwarmUnitT>
class TaskDeque : public std::deque<PtrTask<SwarmUnitT>> {}; // TODO rb tree ?

/**
 * @brief Interface of the task manager swarm unit component. This module get,
 * decompose and manage the tasks from swarm or other units. The shell above
 * TaskDeque
 *
 */
template <class ParamsT, class SwarmUnitT>
class ITaskManagerUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
  TaskDeque<SwarmUnitT> _taskDeque;

public:
  ITaskManagerUnitC(SwarmUnitT *u) : IUnitComponent<ParamsT, SwarmUnitT>(u) {}

  /**
   * @brief Add task in back of deque. The task will be completed last.
   *
   */
  virtual void add_task_in_back(PtrTask<SwarmUnitT> t) {
    _taskDeque.push_back(t);
  }
  /**
   * @brief Add task in front of deque. The task will be completed first.
   *
   */
  virtual void add_task_in_front(PtrTask<SwarmUnitT> t) {
    _taskDeque.push_front(t);
  }
  /**
   * @brief Add task in back of deque. The task will be completed last.
   *
   */
  template <uint32_t Lvl, typename TaskParamsT>
  void add_task_in_back(const ITask<Lvl, SwarmUnitT, TaskParamsT> &t) {
    add_task_in_back(PtrTask<SwarmUnitT>(t));
  }
  /**
   * @brief Add task in front of deque. The task will be completed first.
   *
   */
  template <uint32_t Lvl, typename TaskParamsT>
  void add_task_in_front(const ITask<Lvl, SwarmUnitT, TaskParamsT> &t) {
    add_task_in_front(PtrTask<SwarmUnitT>(t));
  }

  /**
   * @brief decompose task on front and add subtasks to front of deque
   *
   * @return std::pair<typename TaskDeque<SwarmUnitT>::const_iterator,
   * typename TaskDeque<SwarmUnitT>::const_iterator> - pair of const iterators
   * in deque, first iterator - the initial task(was it the front), second
   * iterator - the last task in decomposed subtasks
   */
  std::pair<typename TaskDeque<SwarmUnitT>::const_iterator,
            typename TaskDeque<SwarmUnitT>::const_iterator>
  decompose_front() {
    std::pair<typename TaskDeque<SwarmUnitT>::const_iterator,
              typename TaskDeque<SwarmUnitT>::const_iterator>
        res;
    auto initial_task = _taskDeque.front();
    auto task_to_decompose = initial_task;
    res.first = _taskDeque.begin();
    full_decompose_task(initial_task);
    res.second = _taskDeque.begin();
    return res;
  }
  PtrTask<SwarmUnitT> decompose_task(PtrTask<SwarmUnitT> &task) const;

  template <uint32_t Lvl, class TaskParamsT, class ReturnTaskParamsT>
  const ITask<Lvl - 1, SwarmUnitT, ReturnTaskParamsT> &
  decompose_task(const ITask<Lvl, SwarmUnitT, TaskParamsT>) const;

  void full_decompose_task(PtrTask<SwarmUnitT> task) {
    while (task.is_full_decomposed) {
      // decompose
    }
  }

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