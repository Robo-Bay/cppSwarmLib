#include "Params.hpp"
#include "SwarmUnit.hpp"
#include "UnitComponent/ICommunicationC.hpp"
#include "UnitComponent/IExecutorC.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
int main() {
  swarm::EmptySwarmUnit u;
  swarm::BasicSwarmUnit<swarm::EmptyParams, swarm::EmptyTaskManagerC,
                        swarm::EmptyCommunicationC, swarm::EmptyExecutorC>
      a;
}
