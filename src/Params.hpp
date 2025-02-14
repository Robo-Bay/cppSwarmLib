#pragma once
namespace swarm {
/**
 * @brief the interface of swarm unit or component params.
 * TODO maybe JSON?
 */
struct IParams {
  IParams();
  IParams(auto...);
};

} // namespace swarm