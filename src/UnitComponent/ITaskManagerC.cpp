#include "ITaskManagerC.hpp"
using namespace swarm;

template <class ParmsT, class SwarmUnitT>
ITaskManagerUnitC<ParmsT, SwarmUnitT>::~ITaskManagerUnitC() = default;

template <class ParmsT, class SwarmUnitT>
void ITaskManagerUnitC<ParmsT, SwarmUnitT>::add_task(TaskT task,
                                                     bool add_int_back) {
  if (add_int_back) {
    _tasks_deque.push_back(task);
  } else {
    _tasks_deque.push_front(task);
  }
}