#pragma once
namespace swarm {
/**
 * @brief the interface of swarm unit or component params.
 * TODO maybe JSON?
 */
struct IParams {
  IParams();
  virtual ~IParams() = default;
};
using EmptyParams = IParams;

} // namespace swarm