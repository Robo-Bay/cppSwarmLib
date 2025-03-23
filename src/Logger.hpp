#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <string>

namespace swarm {
    namespace logging = boost::log;
    namespace sinks = boost::log::sinks;
    namespace expr = boost::log::expressions;
    namespace keywords = boost::log::keywords;

    class ILogger {
    protected:
        const std::string name;

    public:
        ILogger(const std::string &name = "") : name(name) {}
        virtual void info(const std::string &, const std::string &from = "") = 0;
        virtual void warning(const std::string &, const std::string &from = "") = 0;
        virtual void error(const std::string &, const std::string &from = "") = 0;
        virtual void debug(const std::string &, const std::string &from = "") = 0;
        virtual void fatal(const std::string &, const std::string &from = "") = 0;
        virtual void init() {}
        virtual void iter() {}
        [[nodiscard]] const std::string &get_name() const { return name; }
        virtual ~ILogger() = default;
    };
    class EmptyLogger : public ILogger {
    public:
        EmptyLogger(const std::string &name = "") : ILogger(name) {}

        void init() override {}
        void iter() override {}
        void info(const std::string &, const std::string & /*from*/) override {}
        void warning(const std::string &, const std::string & /*from*/) override {}
        void error(const std::string &, const std::string & /*from*/) override {}
        void debug(const std::string &, const std::string & /*from*/) override {}
        void fatal(const std::string &, const std::string & /*from*/) override {}
    };

    class BaseLogger : public ILogger {
    private:
        using file_sink_t = sinks::synchronous_sink<sinks::text_file_backend>; // Исправленный тип
        using console_sink_t = sinks::synchronous_sink<sinks::text_ostream_backend>;

        boost::shared_ptr<file_sink_t> file_sink;
        boost::shared_ptr<console_sink_t> console_sink;
        logging::core_ptr core;
        logging::sources::severity_logger<logging::trivial::severity_level> lg;

    public:
        BaseLogger(const std::string &logger_name, bool enable_console = true) :
            ILogger(logger_name), core(logging::core::get()) {
            file_sink = init_file_sink("logs/_" + logger_name + "_.log");
            if (enable_console) {
                console_sink = init_console_sink();
            }
            logging::add_common_attributes();
        }

        virtual ~BaseLogger() {
            // Удаляем sinks из core
            if (file_sink)
                core->remove_sink(file_sink);
            if (console_sink)
                core->remove_sink(console_sink);
        }

        void info(const std::string &msg, const std::string &from = "") { log(logging::trivial::info, msg, from); }
        void warning(const std::string &msg, const std::string &from = "") {
            log(logging::trivial::warning, msg, from);
        }
        void error(const std::string &msg, const std::string &from = "") { log(logging::trivial::error, msg, from); }
        void debug(const std::string &msg, const std::string &from = "") { log(logging::trivial::debug, msg, from); }
        void fatal(const std::string &msg, const std::string &from = "") { log(logging::trivial::fatal, msg, from); }

    private:
        template<typename SinkT>
        void set_formatter(SinkT &sink) {
            sink->set_formatter(expr::stream
                                << "["
                                << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                                << "] "
                                << "[" << logging::trivial::severity << "] "
                                << "[!" << this->name << "] "
                                << "[@" << expr::attr<std::string>("From") << "] " << expr::smessage);
        }
        boost::shared_ptr<file_sink_t> init_file_sink(const std::string &filename) {
            auto backend = boost::make_shared<sinks::text_file_backend>(keywords::file_name = filename);
            auto sink = boost::make_shared<file_sink_t>(backend);

            set_formatter(sink);
            sink->set_filter(expr::attr<std::string>("LoggerName") == name);
            core->add_sink(sink);
            return sink;
        }

        boost::shared_ptr<console_sink_t> init_console_sink() {
            auto backend = boost::make_shared<sinks::text_ostream_backend>();
            backend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter()));
            auto sink = boost::make_shared<console_sink_t>(backend);

            set_formatter(sink);
            sink->set_filter(expr::attr<std::string>("LoggerName") == name);
            core->add_sink(sink);
            return sink;
        }

        void log(logging::trivial::severity_level level, const std::string &msg, const std::string &from) {
            BOOST_LOG_SCOPED_THREAD_TAG("LoggerName", name);
            BOOST_LOG_SCOPED_THREAD_ATTR("From", logging::attributes::constant<std::string>(from));
            BOOST_LOG_SEV(lg, level) << msg;
        }
    };
} // namespace swarm
