#pragma once
#include "IUnitComponent.hpp"
namespace swarm {

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
} // namespace swarm