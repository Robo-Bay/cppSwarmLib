#pragma once
#include "Params.hpp"
#include "UnitComponent/ICommunicationC.hpp"
#include "UnitComponent/IExecutorC.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
#include <type_traits>
namespace swarm {
template <class ParamsT, class SwarmUnitT> class ICommunicationUnitC;
template <class ParamsT, class SwarmUnitT> class IExecutorUnitC;
template <class ParamsT, class SwarmUnitT> class ITaskManagerUnitC;

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
template <class UnitParamsT, template <class> class TaskManagerCTT,
          template <class> class CommunicationCTT,
          template <class> class ExecutorCTT>
class BasicSwarmUnit {
public:
  using UnitT = BasicSwarmUnit<UnitParamsT, TaskManagerCTT, CommunicationCTT,
                               ExecutorCTT>;
  using _TaskManagerT = TaskManagerCTT<UnitT>;
  using _CommunicationT = CommunicationCTT<UnitT>;
  using _ExecutorT = ExecutorCTT<UnitT>;

private:
  static_assert(std::is_base_of<IParams, UnitParamsT>::value,
                "UnitParams must be derived from IParams");

  _TaskManagerT *_taskManagerC;
  _CommunicationT *_communicationC;
  _ExecutorT *_executorC;
  UnitParamsT _params;

public:
  BasicSwarmUnit()
      : _taskManagerC(new _TaskManagerT(this)),
        _communicationC(new _CommunicationT(this)),
        _executorC(new _ExecutorT(this)) {}
  void init() {
    _taskManagerC->init();
    _communicationC->init();
    _executorC->init();
  }
  void iter() {
    _taskManagerC->iter();
    _communicationC->iter();
    _executorC->iter();
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
