#pragma once
#include "UnitComponent/ICommunicationC.hpp"
#include "UnitComponent/IExecutorC.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
#include <memory>
#include <type_traits>
namespace swarm {

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
