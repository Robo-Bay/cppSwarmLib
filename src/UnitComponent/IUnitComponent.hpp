#pragma once
#include "../Params.hpp"
#include "../SwarmUnit.hpp"
#include <memory>
#include <type_traits>
namespace swarm {
template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
class BasicSwarmUnit;

/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class SwarmUnitT, class ParamsT> class IUnitComponent {
  static_assert(std::is_base_of<IParams, ParamsT>::value,
                "Params must be derived of IParams");
  // static_assert(
  //     std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
  //                                    SwarmUnitT::template TaskManagerCT,
  //                                    SwarmUnitT::template CommunicationCT,
  //                                    SwarmUnitT::template ExecutorCT>,
  //                     SwarmUnitT>::value,
  //     "SwarmUnitT must be derived from BasicSwarmUnit");

  SwarmUnitT *_unit;

public:
  IUnitComponent(SwarmUnitT *const);
  IUnitComponent(SwarmUnitT *const, const ParamsT &);
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
  virtual ~IUnitComponent() = 0;
};

template <typename SwarmUnitT, typename ParamsT = EmptyParams>
class EmptyUnitComponent : public IUnitComponent<SwarmUnitT, ParamsT> {
  void init() override;
  void iter() override;
  ~EmptyUnitComponent() override = default;
};
} // namespace swarm