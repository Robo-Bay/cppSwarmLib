#pragma once
#include <cppSwarmLib/Swarm.hpp>
namespace swarm {

/**
 * @brief The parasm of swarm of particles
 *
 */
template <std::size_t Dim> struct SOPParams : public IParams {
  std::array<std::pair<double, double>, Dim> Limits;
  double omega = 1;
  double phi_p = 1;
  double phi_g = 1;
};

template <std::size_t Dim>
class SwarmOfParticles : public Swarm<SwarmVectorContainer, SOPParams<Dim>> {
public:
  std::function<double(const std::array<double, Dim> &)> Func;
};
} // namespace swarm
