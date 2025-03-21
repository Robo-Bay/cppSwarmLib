#include <cppSwarmLib/Random.hpp>
#include <cppSwarmLib/Swarm.hpp>
#include <cppSwarmLib/SwarmOfParticles/StopCriteria.hpp>
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmOfParticles/UnitParticl.hpp>
#include <cppSwarmLib/SwarmUnit.hpp>
#include <functional>
#include <numeric>
constexpr std::size_t Dim = 2;
int main() {
    auto p = swarm::SOPParams<Dim>();
    p.Limits[0] = {-100.0, 100.0};
    p.Limits[1] = {-100.0, 100.0};

    auto f = [](const std::array<double, Dim> &a) { return std::sin(a[0]) + std::cos(a[1]); };
    swarm::SwarmOfParticles<Dim> sw(f, p, 25, swarm::FindBestCriteria<Dim>(2));
    sw.init<swarm::ParticlUnit<Dim, std::greater<double>>>(true);
    char imp;
    do {
        sw.iter();
    } while (!sw.is_stoped());
}
