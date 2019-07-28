#include "logger.h"

#include "../time/time.h"

namespace FW::Core {
Logger::Logger() {}

void Logger::start(const std::string& file) {}

void Logger::debug(const std::string& what, const std::string& caller,
                   const std::string& trace) {
  log(Level::Debug, what, caller, trace);
}

void Logger::info(const std::string& what, const std::string& caller,
                  const std::string& trace) {
  log(Level::Info, what, caller, trace);
}

void Logger::warning(const std::string& what, const std::string& caller,
                     const std::string& trace) {
  log(Level::Warn, what, caller, trace);
}

void Logger::error(const std::string& what, const std::string& caller,
                   const std::string& trace) {
  log(Level::Error, what, caller, trace);
}

void Logger::fatal(const std::string& what, const std::string& caller,
                   const std::string& trace) {
  log(Level::Fatal, what, caller, trace);
}

void Logger::log(Level level, const std::string& message,
                 const std::string& caller, const std::string& trace) {}

}  // namespace FW::Core
