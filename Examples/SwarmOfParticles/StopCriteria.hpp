#pragma once
#include <cmath>
#include "SwarmOfParticles.hpp"

namespace swarm {
    template<std::size_t Dim>
    class SwarmOfParticles;
    template<size_t Dim>
    struct StopCriteria {
        virtual bool operator()(const SwarmOfParticles<Dim> &sw) const { return false; }
        friend class SwarmOfParticles<Dim>;
    };

    template<size_t Dim>
    struct FindBestCriteria : public StopCriteria<Dim> {
    private:
        double _best;
        double accuracy;
    public:
        FindBestCriteria(double best, double accuracy = 0.0001) : _best(best), accuracy(accuracy) {}
        bool operator()(const SwarmOfParticles<Dim> &sw) const {
            if (sw._Params.BestVal == _best || std::abs(sw._Params.BestVal - _best) <= accuracy) {
                return true;
            }
            return false;
        }
        friend class SwarmOfParticles<Dim>;
    };

} // namespace swarm
