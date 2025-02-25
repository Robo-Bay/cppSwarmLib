#include <boost/version.hpp>
#include <cassert>
#include <cppSwarmLib/src/Swarm.hpp>
#include <cppSwarmLib/src/SwarmUnit.hpp>
#include <cppSwarmLib/src/Tasks/ITask.hpp>
#include <iostream>

using namespace swarm;
int main() {
  for (int i = 0; i < MaximumTaskLvl; i++) {
    IBaseTask<EmptySwarmUnit> t(i, nullptr, EmptyTaskParams());
    assert(t.get_lvl() == i);
  }
}
