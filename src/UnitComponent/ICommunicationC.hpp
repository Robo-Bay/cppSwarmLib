#pragma once
#include "IUnitComponent.hpp"
namespace swarm {

/**
 * @brief Interface of the communication swarm unit component. In this component
 * unit communicate with other units and swarm.
 *
 */
template <class SwarmUnitT, class ParamsT>
class ICommunicationUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  virtual ~ICommunicationUnitC() = 0;
};
template <typename SwarmUnitT, typename ParamsT = EmptyParams>
class EmptyCommunicationC : public EmptyUnitComponent<SwarmUnitT, ParamsT> {
public:
  ~EmptyCommunicationC() final = default;
};
} // namespace swarm