#pragma once
#include <cppSwarmLib/Params.hpp>
#include <cppSwarmLib/Swarm.hpp>
#include "StopCriteria.hpp"
#include "SwarmOfParticles.hpp"
#include "UnitParticl.hpp"
namespace swarm {

    template<std::size_t Dim>
    class SwarmOfParticles;

    template<std::size_t Dim>
    struct StopCriteria;
    template<size_t Dim>
    struct FindBestCriteria;
    /**
     * @brief The parasm of swarm of particles
     *
     */
    template<std::size_t Dim>
    struct SOPParams : public IParams {
        std::array<std::pair<double, double>, Dim> Limits;
        double omega = 0.729;
        double phi_p = 0.149445;
        double phi_g = 0.149445;

        std::array<double, Dim> BestPos{};
        double BestVal{};
    };

    template<std::size_t Dim>
    class SwarmOfParticles : public Swarm<SwarmVectorContainer, SOPParams<Dim>, IParticlUnit<Dim>> {
        using _Base = Swarm<SwarmVectorContainer, SOPParams<Dim>, IParticlUnit<Dim>>;
        const StopCriteria<Dim> &_stop_criteria;
        bool stopped;

    public:
        std::function<double(const std::array<double, Dim> &)> Func;
        SwarmOfParticles(std::function<double(const std::array<double, Dim> &)> func,
                         const SOPParams<Dim> &p = SOPParams<Dim>(), std::size_t sz = 0,
                         const StopCriteria<Dim> &stop_criteria = StopCriteria<Dim>()) :
            Func(func), _stop_criteria(stop_criteria), _Base(p, sz) {}
        void init() final { _Base::init(); }

        template<typename T>
        void init(bool create_reserve_units = true) {
            if (create_reserve_units) {
                auto count = _Base::_Units.reserved_size() - _Base::_Units.size();
                for (std::size_t i = 0; i < count; ++i) {
                    _Base::_Units.add_unit(SwarmUnitLink<IParticlUnit<Dim>>(
                            new T(_Base::_Params, Func, _Base::_Params.BestPos, _Base::_Params.BestVal)));
                }
            }
            init();
        }
        void iter() final {
            _Base::iter();
            stopped = _stop_criteria(*this);
        }
        bool is_stoped() const { return stopped; }
        friend struct StopCriteria<Dim>;
        friend struct FindBestCriteria<Dim>;
    };
} // namespace swarm
