#pragma once
#include "IUnitComponent.hpp"
namespace swarm {
/**
 * @brief Interface of the executive swarm unit module. This module is execute
 * tasks from task manager module.
 *
 */
template <class SwarmUnitT, class ParamsT>
class IExecutorUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  virtual ~IExecutorUnitC() = 0;
};
template <typename SwarmUnitT, typename ParamsT = EmptyParams>
class EmptyExecutorC : public EmptyUnitComponent<SwarmUnitT, ParamsT> {
public:
  ~EmptyExecutorC() final = default;
};
} // namespace swarm