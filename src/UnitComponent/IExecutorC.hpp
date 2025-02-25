#pragma once
#include "IUnitComponent.hpp"
namespace swarm {
template <class ParamsT, class SwarmUnitT> class IUnitComponent;

/**
 * @brief Interface of the executive swarm unit module. This module is execute
 * tasks from task manager module.
* TODO vasia
 *
 */
template <class ParamsT, class SwarmUnitT>
class IExecutorUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
  IBaseTask<SwarmUnitT>& current_task;
public:
  IExecutorUnitC(SwarmUnitT *u) : IUnitComponent<ParamsT, SwarmUnitT>(u){}
  virtual ~IExecutorUnitC() = default;
};

template <class SwarmUnitT>
class EmptyExecutorC : public IExecutorUnitC<EmptyParams, SwarmUnitT> {
public:
  EmptyExecutorC(SwarmUnitT *u) : IExecutorUnitC<EmptyParams, SwarmUnitT>(u){}
  ~EmptyExecutorC() override = default;
  void init() final {};
  void iter() final {};
};

} // namespace swarm