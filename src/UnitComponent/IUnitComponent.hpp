#pragma once
#include "../Params.hpp"
#include "../SwarmUnit.hpp"
#include <type_traits>
namespace swarm {
template <class SwarmUnitT> class EmptyUnitComponent;
/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class ParamsT, class SwarmUnitT> class IUnitComponent {
  // static_assert(std::is_base_of<IParams, ParamsT>::value,
  //               "Params must be derived of IParams");
  // static_assert(
  //     std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
  //                                    SwarmUnitT::template CommunicationCT,
  //                                    SwarmUnitT::template TaskManagerCT,
  //                                    SwarmUnitT::template ExecutorCT>,
  //                     SwarmUnitT>::value,
  //     "SwarnUnitT must be derived of BasicSwarmUnit");
  ParamsT _params;
  SwarmUnitT *_unit;

public:
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