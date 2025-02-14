
namespace swarm {
/**
 * @brief The basic interface of the basic swarm unit component
 *
 */
class IUnitComponent {
public:
  /**
   * @brief init component, then can iteration
   *
   */
  virtual void init() = 0;
  /**
   * @brief do one iteration of swarm component
   *
   */
  virtual void iter() = 0;
  virtual ~IUnitComponent();
};
/**
 * @brief Interface of the communication swarm unit component. In this component
 * unit communicate with other units and swarm.
 *
 */
class ICommunicationUnitC : IUnitComponent {
public:
  virtual ~ICommunicationUnitC();
};
/**
 * @brief Interface of the task manager swarm unit component. This module get,
 * decompose and manage the tasks from swarm or other units.
 *
 */
class ITaskManagerUnitC : IUnitComponent {
public:
  virtual ~ITaskManagerUnitC();
};
/**
 * @brief Interface ot the executive swarm unit module. This module is execute
 * tasks from task manager module.

 */
class IExecutorUnitC : IUnitComponent {
public:
  virtual ~IExecutorUnitC();
};
class BasicSwarmUnit {};

} // namespace swarm
