#include "ITask.hpp"
using namespace swarm;

template <typename SwarmUnitT, typename TaskParamsT>
IBaseTask<SwarmUnitT, TaskParamsT>::IBaseTask(SwarmUnitT *unit,
                                              const TaskParamsT &params)
    : _unit(unit), _params(params) {}