#pragma once
#include "../Params.hpp"
#include "../SwarmUnit.hpp"
#include <type_traits>
namespace swarm {
class ISwarmUnit;
/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class ParamsT, class SwarmUnitT>
class IUnitComponent : public Parameterizable<ParamsT> {

  // static_assert(std::is_base_of<ISwarmUnit SwarmUnitT>::value,
  //               "SwarmUnitT must be derived from ISwarmUnit");
  static_assert(std::is_base_of<ISwarmUnit, SwarmUnitT>::value, "");

protected:
  ParamsT _Params;
  SwarmUnitT *_U;

public:
  using _P = ParamsT;
  IUnitComponent(SwarmUnitT *u) : _U(u) {};
  IUnitComponent(SwarmUnitT *u, const ParamsT &p) : _U(u), _Params(p) {}
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
  friend SwarmUnitT;
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