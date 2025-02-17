#pragma once
#include "IUnitComponent.hpp"
namespace swarm {
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
} // namespace swarm