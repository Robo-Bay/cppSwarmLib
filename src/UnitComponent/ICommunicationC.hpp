#pragma once
#include "IUnitComponent.hpp"
namespace swarm {
template <class ParamsT, class SwarmUnitT> class IUnitComponent;
/**
 * @brief Interface of the communication swarm unit component. In this component
 * unit communicate with other units and swarm.
 *
 */
template <class ParamsT, class SwarmUnitT>
class ICommunicationUnitC : public IUnitComponent<ParamsT, SwarmUnitT> {
public:
  ICommunicationUnitC(SwarmUnitT *u) : IUnitComponent<ParamsT, SwarmUnitT>(u) {}
  virtual ~ICommunicationUnitC() = default;
};
template <class SwarmUnitT>
class EmptyCommunicationC
    : public ICommunicationUnitC<EmptyParams, SwarmUnitT> {
public:
  EmptyCommunicationC(SwarmUnitT *u)
      : ICommunicationUnitC<EmptyParams, SwarmUnitT>(u) {}
  ~EmptyCommunicationC() override = default;
  void init() final {};
  void iter() final {};
};
} // namespace swarm