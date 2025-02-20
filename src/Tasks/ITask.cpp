#include "ITask.hpp"
using namespace swarm;

template <typename SwarmUnitT>
IBaseTask<SwarmUnitT>::IBaseTask(SwarmUnitT *unit, const ITaskParams &params)
    : _unit(unit), _params(params) {}