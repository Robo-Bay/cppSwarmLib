#pragma once
#include "IUnitComponent.hpp"
namespace swarm {
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
} // namespace swarm