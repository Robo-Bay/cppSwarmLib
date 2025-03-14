#pragma once
#include <cppSwarmLib/Params.hpp>
#include <cppSwarmLib/Swarm.hpp>
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmOfParticles/UnitParticl.hpp>
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
class SwarmOfParticles
    : public Swarm<SwarmVectorContainer, SOPParams<Dim>, IParticlUnit<Dim>> {
  using _Base = Swarm<SwarmVectorContainer, SOPParams<Dim>, IParticlUnit<Dim>>;

public:
  std::function<double(const std::array<double, Dim> &)> Func;
  SwarmOfParticles(std::function<double(const std::array<double, Dim> &)> func,
                   const SOPParams<Dim> &p = SOPParams<Dim>(),
                   std::size_t sz = 0)
      : Func(func), _Base(p, sz) {}
  void init() final { _Base::init(); }

  template <typename T> void init(bool create_reserve_units = true) {
    if (create_reserve_units) {
      auto count = _Base::_Units.reserved_size() - _Base::_Units.size();
      for (std::size_t i = 0; i < count; ++i) {
        _Base::_Units.add_unit(
            SwarmUnitLink<IParticlUnit<Dim>>(new T(_Base::_Params, Func)));
      }
    }
    init();
  }
};
} // namespace swarm
