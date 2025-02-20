#include "IUnitComponent.hpp"

using namespace swarm;
template <class SwarmUnitT, class ParamsT>
IUnitComponent<SwarmUnitT, ParamsT>::IUnitComponent(SwarmUnitT *const unit,
                                                    const ParmsT &params)
    : _params(params), _unit(unit) {}
template <class SwarmUnitT, class ParamsT>
IUnitComponent<SwarmUnitT, ParamsT>::IUnitComponent(SwarmUnitT *const unit)
    : _unit(unit) {}

template <class SwarmUnitT, class ParamsT>
IUnitComponent<SwarmUnitT, ParamsT>::~IUnitComponent() = default;

template <typename SwarmUnitT, typename ParamsT>
void EmptyUnitComponent<SwarmUnitT, ParamsT>::init() {}
template <typename SwarmUnitT, typename ParamsT>
void EmptyUnitComponent<SwarmUnitT, ParamsT>::iter() {}