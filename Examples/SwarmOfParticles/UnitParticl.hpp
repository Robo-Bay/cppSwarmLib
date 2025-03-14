#pragma once
#include <cppSwarmLib/Params.hpp>
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmUnit.hpp>
#include <cppSwarmLib/UnitComponent/IExecutorC.hpp>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <type_traits>

namespace swarm {
template <std::size_t Dim> struct SOPParams;
template <std::size_t Dim> class IParticlUnit : public ISwarmUnit {
public:
  virtual const std::array<double, Dim> &getPosition() = 0;
  virtual double getCurValue() = 0;
  virtual const std::array<double, Dim> &getPositionMaxVal() = 0;
  virtual double getMaxVal() = 0;
};
template <std::size_t Dim> class ParticlUnit;
template <typename UT>
class ParticlExecutor : public IExecutorUnitC<EmptyParams, UT> {
public:
  void init() override {}
  void iter() override {}
  ParticlExecutor(UT *u) : IExecutorUnitC<EmptyParams, UT>(u) {}
};

template <std::size_t Dim>
class ParticlExecutor<ParticlUnit<Dim>>
    : public IExecutorUnitC<EmptyParams, ParticlUnit<Dim>> {

  inline double rnd(double min, double max) const {
    return SwarmStaticRandom::rnd(min, max);
  }

public:
  void _setRandomPosition() {
    for (auto i = 0; i < Dim; ++i) {
      ParticlUnit<Dim> &u = *this->_U;
      u._Cur_pos[i] =
          rnd(u._params->Limits[i].first, u._params->Limits[i].second);
    }
  }
  void init() override { _setRandomPosition(); }
  void iter() override {}
  ParticlExecutor(ParticlUnit<Dim> *u)
      : IExecutorUnitC<EmptyParams, ParticlUnit<Dim>>(u) {}
  friend ParticlUnit<Dim>;
};

template <std::size_t Dim>
class ParticlUnit
    : public BasicSwarmUnit<
          ParticlUnit<Dim>, LinkToGlobalParams<SOPParams<Dim>>,
          EmptyTaskManagerC, EmptyCommunicationC, ParticlExecutor>,
      public virtual IParticlUnit<Dim> {
  std::array<double, Dim> _Cur_pos;
  std::function<double(const std::array<double, Dim> &)> _Func;

  double _MaxVal;
  std::array<double, Dim> _PositionMaxVal;

public:
  using _Base =
      BasicSwarmUnit<ParticlUnit<Dim>, LinkToGlobalParams<SOPParams<Dim>>,
                     EmptyTaskManagerC, EmptyCommunicationC, ParticlExecutor>;
  ParticlUnit(const SOPParams<Dim> &p,
              std::function<double(const std::array<double, Dim> &)> func)
      : _Func(func),
        BasicSwarmUnit<ParticlUnit<Dim>, LinkToGlobalParams<SOPParams<Dim>>,
                       EmptyTaskManagerC, EmptyCommunicationC, ParticlExecutor>(
            LinkToGlobalParams<SOPParams<Dim>>(p)) {}

  const std::array<double, Dim> &getPosition() override { return _Cur_pos; }
  double getCurValue() override { return _Func(_Cur_pos); }
  const std::array<double, Dim> &getPositionMaxVal() override {
    return _PositionMaxVal;
  }
  double getMaxVal() override { return _MaxVal; }

  void init() override { /*  random spawn*/
    _Base::init();
    // log("init");
  }
  void iter() override {
    // iter TaskManager
    // iter executor
  }

private:
  void log(const std::string &mes) const {
    std::cout << mes << " :";
    std::cout.precision(3);
    for (auto i = 0; i < Dim; ++i) {
      std::cout << " : " << std::setw(8) << _Cur_pos[i];
    }
    std::cout << " -> " << _Func(_Cur_pos) << std::endl;
  }
  friend ParticlExecutor<ParticlUnit<Dim>>;
};
} // namespace swarm