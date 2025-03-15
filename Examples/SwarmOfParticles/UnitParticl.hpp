#pragma once
#include "VectorsOp.hpp"
#include <array>
#include <cppSwarmLib/Params.hpp>
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmUnit.hpp>
#include <cppSwarmLib/UnitComponent/IExecutorC.hpp>
#include <functional>
#include <iomanip>
#include <iostream>

namespace swarm {
template <std::size_t Dim> struct SOPParams;
template <std::size_t Dim, class Compare = std::less<double>> class ParticlUnit;
template <std::size_t Dim> class IParticlUnit : public ISwarmUnit {
public:
  virtual const std::array<double, Dim> &getPosition() = 0;
  virtual double getCurValue() = 0;
  virtual const std::array<double, Dim> &getPositionMaxVal() = 0;
  virtual double getMaxVal() = 0;
};
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
  void _setRandomVel() {
    for (auto i = 0; i < Dim; ++i) {
      ParticlUnit<Dim> &u = *this->_U;
      u._Vel[i] = rnd(u._params->Limits[i].first, u._params->Limits[i].second);
    }
  }

  void init() override {
    _setRandomPosition();
    _setRandomVel();
  }
  void iter() override {}
  ParticlExecutor(ParticlUnit<Dim> *u)
      : IExecutorUnitC<EmptyParams, ParticlUnit<Dim>>(u) {}
  friend ParticlUnit<Dim>;
};

template <std::size_t Dim, class Compare>
class ParticlUnit
    : public BasicSwarmUnit<
          ParticlUnit<Dim>, LinkToGlobalParams<SOPParams<Dim>>,
          EmptyTaskManagerC, EmptyCommunicationC, ParticlExecutor>,
      public virtual IParticlUnit<Dim> {
#define prms _Base::_params
  std::array<double, Dim> _Cur_pos{};
  std::array<double, Dim> _Vel{};
  std::function<double(const std::array<double, Dim> &)> _Func;

  double _MaxVal{};
  std::array<double, Dim> _PositionMaxVal{};

  Compare _Comp{};
  std::array<double, Dim> &BestPos;
  double &BestVal;

public:
  using _Base =
      BasicSwarmUnit<ParticlUnit<Dim>, LinkToGlobalParams<SOPParams<Dim>>,
                     EmptyTaskManagerC, EmptyCommunicationC, ParticlExecutor>;
  ParticlUnit(const SOPParams<Dim> &p,
              std::function<double(const std::array<double, Dim> &)> func,
              std::array<double, Dim> &BestPos, double &BestVal)
      : _Func(func), BestPos(BestPos), BestVal(BestVal),
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
    log("init");
  }
  void iter() override {
    update_max(_Func(_Cur_pos));
    update_pos();
    log("iter");
  }

private:
  void update_max(double curx) {
    if (_Comp(curx, _MaxVal)) {
      _PositionMaxVal = _Cur_pos;
      _MaxVal = curx;
    }
    if (_Comp(curx, BestVal)) {
      BestPos = _Cur_pos;
      BestVal = _MaxVal;
    }
  }
  void update_pos() {
    std::array<double, Dim> rp;
    std::array<double, Dim> rg;
    for (auto i = 0; i < Dim; ++i) {
      rp[i] = rnd();
      rg[i] = rnd();
    }
    _Vel = prms->omega * _Vel +
           (prms->phi_p * rp * (_PositionMaxVal - _Cur_pos)) +
           (prms->phi_g * rg * (BestPos - _Cur_pos));
    _Cur_pos = clamp(_Cur_pos + _Vel, prms->Limits);
  }
  inline double rnd(double min = 0, double max = 1) const {
    return SwarmStaticRandom::rnd(min, max);
  }
  void log(const std::string &mes) const {
    std::cout << mes << " :";
    std::cout.precision(3);
    for (auto i = 0; i < Dim; ++i) {
      std::cout << " : " << std::setw(8) << _Cur_pos[i];
    }
    std::cout << " -> " << _Func(_Cur_pos) << "\t( " << _MaxVal << " )\t";
    for (auto i = 0; i < Dim; ++i) {
      std::cout << " : " << std::setw(8) << _Vel[i];
    }
    std::cout << std::endl;
  }
  friend ParticlExecutor<ParticlUnit<Dim>>;
};

} // namespace swarm