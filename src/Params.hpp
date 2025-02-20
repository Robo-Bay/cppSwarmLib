#pragma once
#include <type_traits>
namespace swarm {
/**
 * @brief the interface of swarm unit or component params.
 * TODO maybe JSON?
 */
struct IParams {
  IParams() = default;
  virtual ~IParams() = 0;
};

struct EmptyParams : public IParams {
  EmptyParams() = default;
  ~EmptyParams() final = default;
};

/**
 * @brief Parameters for the task. They are set once when a task is set.
 *
 */
struct ITaskParams : public IParams {
  ITaskParams() = default;
  virtual ~ITaskParams() = 0;
};
struct EmptyTaskParams : public IParams {
  EmptyTaskParams() = default;
  ~EmptyTaskParams() override = default;
};
} // namespace swarm