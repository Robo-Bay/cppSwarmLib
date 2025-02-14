#pragma once
#include "Params.hpp"
#include <type_traits>
namespace swarm {
template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
class BasicSwarmUnit;

/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class ParamsT, class SwarmUnitT> class IUnitComponent {
  static_assert(std::is_base_of<IParams, ParamsT>::value,
                "Params must be derived of IParams");
  static_assert(
      std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
                                     typename SwarmUnitT::CommunicationCT,
                                     typename SwarmUnitT::TaskManagerCT,
                                     typename SwarmUnitT::ExecutorCT>,
                      SwarmUnitT>::value,
      "SwarnUnitT must be derived of BasicSwarmUnit");
  ParamsT _params;
  SwarmUnitT *_unit;

public:
  IUnitComponent(SwarmUnitT *const, const ParamsT &);
  /**
   * @brief init component, then can iteration
   *
   */
  virtual void init() = 0;
  /**
   * @brief do one iteration of swarm component
   *
   */
  virtual void iter() = 0;
  virtual ~IUnitComponent();
};
/**
 * @brief Interface of the communication swarm unit component. In this component
 * unit communicate with other units and swarm.
 *
 */
template <class ParamsT, class SwarmUnitT>
class ICommunicationUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  virtual ~ICommunicationUnitC();
};

/**
 * @brief Interface of the task manager swarm unit component. This module get,
 * decompose and manage the tasks from swarm or other units.
 *
 */
template <class ParamsT, class SwarmUnitT>
class ITaskManagerUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  virtual ~ITaskManagerUnitC();
};

/**
 * @brief Interface of the executive swarm unit module. This module is execute
 * tasks from task manager module.
 *
 */
template <class ParamsT, class SwarmUnitT>
class IExecutorUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  virtual ~IExecutorUnitC();
};
/**
 * @brief Interface of the basic swarm unit(agent)
 *
 * @tparam CommunicationCT is a communication component of agent. Must be
 * derived from ICommunicationUnitC.
 * @tparam TaskManagerCT is a task manager component of agent. Must be derived
 * from ITaskManagerUnitC.
 * @tparam ExecutorCT is a executor component of agent. Must be derived from
 * IExecutorUnitC.
 */
template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
class BasicSwarmUnit {
  static_assert(std::is_base_of<UnitParamsT, IParams>::value,
                "UnitParams must be derived from IParams");
  // Проверка, что CommunicationCT наследует от ICommunicationUnitC<Params,
  // SwarmUnitT>
  static_assert(
      std::is_base_of<ICommunicationUnitC<typename CommunicationCT::ParamsT,
                                          BasicSwarmUnit>,
                      CommunicationCT>::value,
      "CommunicationCT must be derived from ICommunicationUnitC<ParamsT, "
      "SwarmUnitT>");

  // Проверка, что TaskManagerCT наследует от ITaskManagerUnitC<ParamsT,
  // SwarmUnitT>
  static_assert(
      std::is_base_of<
          ITaskManagerUnitC<typename TaskManagerCT::ParamsT, BasicSwarmUnit>,
          TaskManagerCT>::value,
      "TaskManagerCT must be derived from ITaskManagerUnitC<ParamsT, "
      "SwarmUnitT>");

  // Проверка, что ExecutorCT наследует от IExecutorUnitC<ParamsT, SwarmUnitT>
  static_assert(
      std::is_base_of<
          IExecutorUnitC<typename ExecutorCT::ParamsT, BasicSwarmUnit>,
          ExecutorCT>::value,
      "ExecutorCT must be derived from IExecutorUnitC<ParamsT, SwarmUnitT>");

  CommunicationCT &_communicationC;
  TaskManagerCT &_taskManagerC;
  ExecutorCT &_executorC;
  UnitParamsT _params;

public:
  BasicSwarmUnit();
  BasicSwarmUnit(const UnitParamsT &);
  BasicSwarmUnit(const UnitParamsT &, const CommunicationCT::ParamsT &,
                 const TaskManagerCT::ParamsT &, const ExecutorCT::ParamsT &);
  void init();
  void iter();
  // Деструктор
  virtual ~BasicSwarmUnit();
};

} // namespace swarm
