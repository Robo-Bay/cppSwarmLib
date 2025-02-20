#pragma once
#include "Params.hpp"
#include "UnitComponent/ICommunicationC.hpp"
#include "UnitComponent/IExecutorC.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
#include "UnitComponent/IUnitComponent.hpp"
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
template <class UnitParamsT, template <class, class> class TaskManagerCT,
          template <class, class> class CommunicationCT,
          template <class, class> class ExecutorCT>
class BasicSwarmUnit {
  using UnitT =
      BasicSwarmUnit<UnitParamsT, CommunicationCT, TaskManagerCT, ExecutorCT>;
  static_assert(std::is_base_of<IParams, UnitParamsT>::value,
                "UnitParams must be derived from IParams");
  // Проверка, что ExecutorCT<UnitT> является наследником IExecutorUnitC
  static_assert(
      std::is_base_of<
          IExecutorUnitC<UnitT, ExecutorCT<UnitT>::IUnitComponent::ParamsT>,
          ExecutorCT<UnitT>>::value,
      "ExecutorCT must be derived from IExecutorUnitC");

  // Проверка, что TaskManagerCT<UnitT> является наследником ITaskManagerUnitC
  static_assert(
      std::is_base_of<ITaskManagerUnitC<
                          UnitT, TaskManagerCT<UnitT>::IUnitComponent::ParamsT>,
                      TaskManagerCT<UnitT>>::value,
      "TaskManagerCT must be derived from ITaskManagerUnitC");

  // Проверка, что CommunicationCT<UnitT> является наследником
  // ICommunicationUnitC
  static_assert(std::is_base_of<
                    ICommunicationUnitC<
                        UnitT, CommunicationCT<UnitT>::IUnitComponent::ParamsT>,
                    CommunicationCT<UnitT>>::value,
                "CommunicationCT must be derived from ICommunicationUnitC");
  CommunicationCT &_communicationC;
  TaskManagerCT &_taskManagerC;
  ExecutorCT &_executorC;
  UnitParamsT &_params;

public:
  BasicSwarmUnit();
  BasicSwarmUnit(const UnitParamsT &);
  BasicSwarmUnit(const UnitParamsT &, const CommunicationCT::ParamsT &,
                 const TaskManagerCT::ParamsT &, const ExecutorCT::ParamsT &);
  void virtual init();
  void virtual iter();
  // Деструктор
  virtual ~BasicSwarmUnit() = 0;
};

} // namespace swarm
