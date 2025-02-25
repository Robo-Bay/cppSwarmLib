#pragma once
namespace swarm {
/**
 * @brief the interface of swarm unit or component params.
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
struct ITaskParams : public IParams {
  ITaskParams() = default;
  virtual ~ITaskParams() = default;
};
struct EmptyTaskParams : public ITaskParams {
  EmptyTaskParams() = default;
  ~EmptyTaskParams() = default;
};
} // namespace swarm