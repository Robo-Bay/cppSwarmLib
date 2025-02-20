#include "SwarmUnit.hpp"
using namespace swarm;

template <class UnitParamsT, template <class, class> class TaskManagerCT,
          template <class, class> class CommunicationCT,
          template <class, class> class ExecutorCT>
BasicSwarmUnit<UnitParamsT, TaskManagerCT, CommunicationCT,
               ExecutorCT>::BasicSwarmUnit()
    : _communicationC(*new CommunicationCT(this)),
      _taskManagerC(*new TaskManagerCT(this)),
      _executorC(*new TaskManagerCT(this)) {}

template <class UnitParamsT, template <class, class> class TaskManagerCT,
          template <class, class> class CommunicationCT,
          template <class, class> class ExecutorCT>
BasicSwarmUnit<UnitParamsT, TaskManagerCT, CommunicationCT,
               ExecutorCT>::BasicSwarmUnit(const UnitParamsT &params)
    : _params(params), _communicationC(*new CommunicationCT(this)),
      _taskManagerC(*new TaskManagerCT(this)),
      _executorC(*new ExecutorCT(this)) {}

template <class UnitParamsT, template <class, class> class TaskManagerCT,
          template <class, class> class CommunicationCT,
          template <class, class> class ExecutorCT>
BasicSwarmUnit<UnitParamsT, TaskManagerCT, CommunicationCT,
               ExecutorCT>::BasicSwarmUnit(const UnitParamsT &params,
                                           const CommunicationCT::ParamsT &cpar,
                                           const TaskManagerCT::ParamsT &tpar,
                                           const ExecutorCT::ParamsT &epar)
    : _params(params), _communicationC(*new CommunicationCT(this, cpar)),
      _taskManagerC(*new TaskManagerCT(this, tpar)),
      _executorC(*new ExecutorCT(this, epar)) {}

template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
void BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT,
                    ExecutorCT>::init() {
  _communicationC.init();
  _taskManagerC.init();
  _executorC().init();
}

template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
void BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT,
                    ExecutorCT>::iter() {
  _communicationC.iter();
  _taskManagerC.iter();
  _executorC().iter();
}