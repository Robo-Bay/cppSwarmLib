#pragma once

#include "Params.hpp"
#include "UnitComponent/ICommunicationC.hpp"
#include "UnitComponent/IExecutorC.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
#include <cstdarg>
#include <type_traits>
namespace swarm {
/**
 * @brief Interface of the swarm unit(agent)
 *
 */
class ISwarmUnit {
public:
  ISwarmUnit() = default;
  virtual void init() = 0;
  virtual void iter() = 0;
  virtual ~ISwarmUnit() = default;
};
/**
 * @brief Class of the basic swarm unit(agent)
 *
 * @tparam CommunicationCT is a communication component of agent. Must be
 * derived from ICommunicationUnitC.
 * @tparam TaskManagerCT is a task manager component of agent. Must be derived
 * from ITaskManagerUnitC.
 * @tparam ExecutorCT is a executor component of agent. Must be derived from
 * IExecutorUnitC.
 */
template <class UnitParamsT = EmptyParams,
          template <class> class TaskManagerCTT = EmptyTaskManagerC,
          template <class> class CommunicationCTT = EmptyCommunicationC,
          template <class> class ExecutorCTT = EmptyExecutorC>
class BasicSwarmUnit : public ISwarmUnit, public Parameterizable<UnitParamsT> {
public:
  using UnitT = BasicSwarmUnit<UnitParamsT, TaskManagerCTT, CommunicationCTT,
                               ExecutorCTT>;
  template <class T> using _TMT = TaskManagerCTT<T>;
  template <class T> using _CT = CommunicationCTT<T>;
  template <class T> using _ET = TaskManagerCTT<T>;
  using _TaskManagerT = TaskManagerCTT<UnitT>;
  using _CommunicationT = CommunicationCTT<UnitT>;
  using _ExecutorT = ExecutorCTT<UnitT>;
  using _UnitParamsT = UnitParamsT;

private:
  static_assert(
      std::is_base_of<ITaskManagerUnitC<
                          typename _TaskManagerT::ITaskManagerUnitC::_P, UnitT>,
                      _TaskManagerT>::value,
      "Template TaskManagerCTT must be derived from ITaskManagerUnitC");
  static_assert(
      std::is_base_of<
          ICommunicationUnitC<typename _CommunicationT::ICommunicationUnitC::_P,
                              UnitT>,
          _CommunicationT>::value,
      "Template CommunicationCTT must be derived from ICommunicationUnitC");
  static_assert(
      std::is_base_of<
          IExecutorUnitC<typename _ExecutorT::IExecutorUnitC::_P, UnitT>,
          _ExecutorT>::value,
      "Template ExecutorCTT must be derived from IExecutorUnitC");

  _TaskManagerT &_taskManagerC;
  _CommunicationT &_communicationC;
  _ExecutorT &_executorC;
  UnitParamsT _params;

public:
  BasicSwarmUnit()
      : _taskManagerC(*new _TaskManagerT(this)),
        _communicationC(*new _CommunicationT(this)),
        _executorC(*new _ExecutorT(this)) {}
  BasicSwarmUnit(const UnitParamsT &p)
      : _params(p), _taskManagerC(*new _TaskManagerT(this)),
        _communicationC(*new _CommunicationT(this)),
        _executorC(*new _ExecutorT(this)) {}
  void init() {
    _taskManagerC.init();
    _communicationC.init();
    _executorC.init();
  }
  void iter() {
    _taskManagerC.iter();
    _communicationC.iter();
    _executorC.iter();
  }
  virtual ~BasicSwarmUnit() = default;
};

class EmptySwarmUnit
    : public BasicSwarmUnit<EmptyParams, EmptyTaskManagerC, EmptyCommunicationC,
                            EmptyExecutorC> {
public:
  EmptySwarmUnit()
      : BasicSwarmUnit<EmptyParams, EmptyTaskManagerC, EmptyCommunicationC,
                       EmptyExecutorC>() {}
  ~EmptySwarmUnit() = default;
};
} // namespace swarm
