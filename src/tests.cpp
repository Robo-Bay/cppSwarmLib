#include "Params.hpp"
#include "SwarmUnit.hpp"
#include "UnitComponent/ITaskManagerC.hpp"
#include "UnitComponent/IUnitComponent.hpp"
#include <type_traits>

using namespace swarm;
int main() { 
    BasicSwarmUnit<EmptyParams, EmptyTaskManagerC<typename SwarmUnitT>, class TaskManagerCT, class ExecutorCT>
 }