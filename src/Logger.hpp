#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <string>
namespace swarm {

class ILogger {
public:
  ILogger(const std::string &name = "") {}
  virtual void init() = 0;
  virtual void iter() = 0;
  virtual void info(const std::string &) = 0;
  virtual void warning(const std::string &) = 0;
  virtual void error(const std::string &) = 0;
  virtual void debug(const std::string &) = 0;
  virtual void fatal(const std::string &) = 0;
  virtual const std::string &get_name() = 0;
  virtual ~ILogger() = default;
};
class EmptyLogger : public ILogger {
public:
  EmptyLogger(const std::string &name = "") : ILogger(name) {}

  void init() override {}
  void iter() override {}
  void info(const std::string &) override {}
  void warning(const std::string &) override {}
  void error(const std::string &) override {}
  void debug(const std::string &) override {}
  void fatal(const std::string &) override {}
  const std::string &get_name() override {
    static std::string emptyName;
    return emptyName;
  }
};

class BaseLogger : public ILogger { // TODO boost logger
  bool to_file;
  bool to_console;
  const std::string &name;

public:
  BaseLogger(const std::string &name = "", bool to_file = false,
             bool to_console = false)
      : ILogger(name), to_file(to_file), to_console(to_console), name(name) {}

  virtual ~BaseLogger() override = default;
  const std::string &get_name() override { return name; }
};
class ComponentLogger : public BaseLogger {
public:
  ComponentLogger(const std::string &name = "", bool to_file = false,
                  bool to_console = false)
      : BaseLogger(name, to_file, to_console) {}

  virtual ~ComponentLogger() override = default;
};

class UnitLogger : public BaseLogger {
protected:
  bool to_other_unit;

public:
  UnitLogger(const std::string &name = "", bool to_file = false,
             bool to_console = false, bool to_other_unit = false)
      : BaseLogger(name, to_file, to_console), to_other_unit(to_other_unit) {}

  virtual ~UnitLogger() override = default;
};

class SwarmLogger : public BaseLogger {
public:
  SwarmLogger(const std::string &name = "", bool to_file = false,
              bool to_console = false)
      : BaseLogger(name, to_file, to_console) {}

  virtual ~SwarmLogger() override = default;
};

} // namespace swarm