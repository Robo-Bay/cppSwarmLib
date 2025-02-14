#include "SwarmUnit.hpp"
using namespace swarm;

template <class ParmsT, class SwarmUnitT>
IUnitComponent<ParmsT, SwarmUnitT>::IUnitComponent(SwarmUnitT *const unit,
                                                   const ParmsT &params)
    : _params(params), _unit(unit) {}

template <class ParmsT, class SwarmUnitT>
IUnitComponent<ParmsT, SwarmUnitT>::~IUnitComponent() = default;
template <class ParmsT, class SwarmUnitT>
ICommunicationUnitC<ParmsT, SwarmUnitT>::~ICommunicationUnitC() = default;
template <class ParmsT, class SwarmUnitT>
ITaskManagerUnitC<ParmsT, SwarmUnitT>::~ITaskManagerUnitC() = default;
template <class ParmsT, class SwarmUnitT>
IExecutorUnitC<ParmsT, SwarmUnitT>::~IExecutorUnitC() = default;

template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT,
               ExecutorCT>::BasicSwarmUnit()
    : _communicationC(*new CommunicationCT()),
      _taskManagerC(*new TaskManagerCT()), _executorC(*new TaskManagerCT()) {}

template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT,
               ExecutorCT>::BasicSwarmUnit(const UnitParamsT &params)
    : _params(params) {}

template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT,
               ExecutorCT>::BasicSwarmUnit(const UnitParamsT &params,
                                           const CommunicationCT::ParamsT &cpar,
                                           const TaskManagerCT::ParamsT &tpar,
                                           const ExecutorCT::ParamsT &epar)
    : _params(params), _communicationC(*new CommunicationCT(cpar)),
      _taskManagerC(*new TaskManagerCT(tpar)),
      _executorC(*new ExecutorCT(epar)) {}

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