#pragma once
#include "../Params.hpp"
#include "../SwarmUnit.hpp"
#include <type_traits>
namespace swarm {
template <class UnitParamsT, class CommunicationCT, class TaskManagerCT,
          class ExecutorCT>
class BasicSwarmUnit;

/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
template <class ParamsT, class SwarmUnitT> class IUnitComponent {
  static_assert(std::is_base_of<IParams, ParamsT>::value,
                "Params must be derived of IParams");
  static_assert(
      std::is_base_of<BasicSwarmUnit<typename SwarmUnitT::UnitParamsT,
                                     typename SwarmUnitT::CommunicationCT,
                                     typename SwarmUnitT::TaskManagerCT,
                                     typename SwarmUnitT::ExecutorCT>,
                      SwarmUnitT>::value,
      "SwarnUnitT must be derived of BasicSwarmUnit");
  ParamsT _params;
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
  virtual ~IUnitComponent();
};
} // namespace swarm