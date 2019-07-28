#ifndef FW_CORE_LOGGER_H
#define FW_CORE_LOGGER_H

#include "declarations.h"

#include "../thread/eventmanager.h"
namespace FW::Core {
class Logger : protected FW::Thread::EventManager {
 public:
  struct Config {
    bool showTime = true;
    bool showInfo = true;
    bool showWarning = true;
    bool showError = true;
    bool showFatalError = true;
    bool showDebug = true;
    bool logToConsole = true;
    bool logToFile = true;
    bool debug_enabled = true;
  };

 public:
  enum class Level { Info, Warn, Error, Fatal, Debug };

 public:
  Logger(const std::string& traceInfo);
  virtual ~Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void start(const std::string& logFile);
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
  std::ofstream logFile_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_LOGGER_H