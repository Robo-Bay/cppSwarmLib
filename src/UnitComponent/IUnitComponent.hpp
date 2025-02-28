#pragma once
#include "../Params.hpp"
#include <type_traits>
namespace swarm {
template <class UnitParamsT, template <class> class TaskManagerCTT,
          template <class> class CommunicationCTT,
          template <class> class ExecutorCTT>
class BasicSwarmUnit;
template <class SwarmUnitT> class EmptyUnitComponent;
/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class ParamsT, class SwarmUnitT> class IUnitComponent : public Parameterizable<ParamsT>{

  static_assert(std::is_base_of<typename SwarmUnitT::BasicSwarmUnit::UnitT,
                                SwarmUnitT>::value,
                "SwarmUnitT must be derived from BasicSwarmUnit");
  ParamsT _params;
  SwarmUnitT *_unit;

public:
  using _P = ParamsT;
  IUnitComponent(SwarmUnitT *u) : _unit(u) {};
  IUnitComponent(SwarmUnitT *u, const ParamsT &p) : _unit(u), _params(p) {}
  /**
   * @brief init component, then can iteration
   *
   */
  virtual void init() = 0;
  /**
   * @brief do one iteration of swarm component
   *
   */
  virtual void iter() = 0;
  virtual ~IUnitComponent() = default;
};

template <class SwarmUnitT>
class EmptyUnitComponent : public IUnitComponent<EmptyParams, SwarmUnitT> {
public:
  EmptyUnitComponent(SwarmUnitT *u)
      : IUnitComponent<EmptyParams, SwarmUnitT>(u) {}
  void init() final {};
  void iter() final {};
  ~EmptyUnitComponent() = default;
};

} // namespace swarm