#pragma once
#include <type_traits>
namespace swarm {
/**
 * @brief the interface of configurable parameters.
 * TODO maybe JSON?
 */
struct IParams {
  IParams() = default;
  virtual ~IParams() = default;
};
struct EmptyParams : public IParams {
  EmptyParams() = default;
  ~EmptyParams() = default;
};
/**
 * @brief the inteface of task parameters. The parameters with which we set the
 * task
 *
 */
struct ITaskParams : public IParams {
  ITaskParams() = default;
  virtual ~ITaskParams() = default;
};
struct EmptyTaskParams : public ITaskParams {
  EmptyTaskParams() = default;
  ~EmptyTaskParams() = default;
};

/**
 * @brief interface of parameterizable object.
 *
 * @tparam ParamsT
 */
template <typename ParamsT> struct Parameterizable {
  static_assert(std::is_base_of<IParams, ParamsT>::value,
                "Params must be derived of IParams");
};
} // namespace swarm