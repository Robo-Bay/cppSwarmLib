#include <cppSwarmLib/Random.hpp>
#include <cppSwarmLib/Swarm.hpp>
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmOfParticles/UnitParticl.hpp>
#include <cppSwarmLib/SwarmUnit.hpp>
#include <numeric>
constexpr std::size_t Dim = 3;
int main() {
  auto p = swarm::SOPParams<Dim>();
  p.Limits[0] = {-10.0, 10.0};
  p.Limits[1] = {-20.0, 20.0};
  p.Limits[2] = {-100.0, 100.0};

  auto f = [](const std::array<double, Dim> &a) {
    return std::accumulate(begin(a), end(a), 0.0, [](double acc, double v) {
      return acc + std::abs(v);
    }); // sum of abs
  };
  swarm::SwarmOfParticles<Dim> sw(f, p, 3);
  sw.init<swarm::ParticlUnit<Dim>>(true);
  char imp;
  do {
    sw.iter();
    std::cin >> imp;
  } while (imp == 'y');
}
