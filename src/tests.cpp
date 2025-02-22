#include <cppSwarmLib/src/Swarm.hpp>
int main() {
  swarm::EmptySwarmUnit u;
  swarm::BasicSwarmUnit<swarm::EmptyParams, swarm::EmptyTaskManagerC,
                        swarm::EmptyCommunicationC, swarm::EmptyExecutorC>
      a;
}
