#include "IUnitComponent.hpp"

using namespace swarm;
template <class ParmsT, class SwarmUnitT>
IUnitComponent<ParmsT, SwarmUnitT>::IUnitComponent(SwarmUnitT *const unit,
                                                   const ParmsT &params)
    : _params(params), _unit(unit) {}
template <class ParmsT, class SwarmUnitT>
IUnitComponent<ParmsT, SwarmUnitT>::IUnitComponent(SwarmUnitT *const unit)
    : _unit(unit) {}

template <class ParmsT, class SwarmUnitT>
IUnitComponent<ParmsT, SwarmUnitT>::~IUnitComponent() = default;