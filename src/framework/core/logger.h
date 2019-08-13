#ifndef FW_CORE_LOGGER_H
#define FW_CORE_LOGGER_H

#include "declarations.h"

#include "../thread/eventmanager.h"
namespace FW::Core {
class Logger : protected FW::Thread::EventManager {
 public:
  struct Config {
    bool show_time = true;
    bool show_info = true;
    bool show_warning = true;
    bool show_error = true;
    bool show_fatal_error = true;
    bool show_debug = true;
    bool log_to_console = true;
    bool log_to_file = true;
    bool debug_enabled = true;
  };

 public:
  enum class Level { Info, Warn, Error, Fatal, Debug };

 public:
  Logger(const std::string& traceInfo);
  virtual ~Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void start(const std::string& log_file);
  using EventManager::join;
  using EventManager::terminate;

  void debug(const std::string& message, const std::string& caller = "",
             const std::string& trace = "");
  void info(const std::string& message, const std::string& caller = "",
            const std::string& trace = "");
  void warning(const std::string& message, const std::string& caller = "",
               const std::string& trace = "");
  void error(const std::string& message, const std::string& caller = "",
             const std::string& trace = "");
  void fatal(const std::string& message, const std::string& caller = "",
             const std::string& trace = "");

 protected:
  void log(Level level, const std::string& message,
           const std::string& caller = "", const std::string& trace = "");
  void addEventLog(Level level, const std::string& message,
                   const std::string& caller, const std::string& trace);

 public:
  Config config;

 protected:
  std::ofstream log_file_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_LOGGER_H