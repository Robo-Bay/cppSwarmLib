#pragma once
#include <cppSwarmLib/SwarmOfParticles/SwarmOfParticles.hpp>
#include <cppSwarmLib/SwarmUnit.hpp>
#include <cppSwarmLib/UnitComponent/IExecutorC.hpp>
#include <functional>
#include <iostream>
#include <random>

namespace swarm {
template <std::size_t Dim> struct SOPParams;
template <std::size_t Dim> class IParticlUnit : public ISwarmUnit {
public:
  virtual const std::array<double, Dim> &getPosition() = 0;
  virtual double getCurValue() = 0;
  virtual const std::array<double, Dim> &getPositionMaxVal() = 0;
  virtual double getMaxVal() = 0;
};
struct Random {
  std::random_device rd;
  std::mt19937 gen;

public:
  Random() : gen(rd()) {}
  double operator()(double min = 0, double max = 1) {
    return std::uniform_real_distribution<>(min, max)(gen);
  }
};
template <std::size_t Dim> class ParticlUnit;
template <typename UT>
class ParticlExecutor : public IExecutorUnitC<EmptyParams, UT> {
  void _setRandomPosition() {
    for (auto i = 0; i < UT::Dim; ++i) {
      // _U->_Cur_pos[i] =
      //     _U->_Rnd(_U->_Params->Limits[i].first,
      //     _U->_Params->Limits[i].second);
    }
  }

public:
  ParticlExecutor(UT *u) : IExecutorUnitC<EmptyParams, UT>(u) {}

  void init() override {
    init();
    _setRandomPosition();
  }
  void iter() override {}
};

template <std::size_t Dim>
class ParticlUnit
    : public virtual BasicSwarmUnit<LinkToGlobalParams<SOPParams<Dim>>,
                                    EmptyTaskManagerC, EmptyCommunicationC,
                                    ParticlExecutor>,
      public virtual IParticlUnit<Dim> {
  static inline Random _Rnd;
  std::array<double, Dim> _Cur_pos;
  std::function<double(const std::array<double, Dim> &)> _Func;

  double _MaxVal;
  std::array<double, Dim> _PositionMaxVal;

public:
  ParticlUnit(const SOPParams<Dim> &p,
              std::function<double(const std::array<double, Dim> &)> func)
      : _Func(func),
        BasicSwarmUnit<LinkToGlobalParams<SOPParams<Dim>>, EmptyTaskManagerC,
                       EmptyCommunicationC, ParticlExecutor>(
            LinkToGlobalParams<SOPParams<Dim>>(p)) {}

  const std::array<double, Dim> &getPosition() override { return _Cur_pos; }
  double getCurValue() override { return _Func(_Cur_pos); }
  const std::array<double, Dim> &getPositionMaxVal() override {
    return _PositionMaxVal;
  }
  double getMaxVal() override { return _MaxVal; }

  void init() override { /*  random spawn*/
    init();
    log("init");
  }
  void iter() override {
    // iter TaskManager
    // iter executor
  }

private:
  void log(const std::string &mes) const {
    std::cout << mes << " :";
    std::cout.precision(2);
    for (auto i = 0; i < Dim; ++i) {
      std::cout << " : " << i;
    }
    std::cout << " -> " << _Func(_Cur_pos) << std::endl;
  }
};
} // namespace swarm