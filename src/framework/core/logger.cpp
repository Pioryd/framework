#include "logger.h"

#include "../time/time.h"

namespace FW::Core {
Logger::Logger(const std::string& traceInfo) : EventManager(traceInfo) {}

void Logger::start(const std::string& file) {
  EventManager::start();

  if (config.logToFile) {
    logFile_.open(file.c_str(), std::ios::out | std::ios::trunc);
    if (!logFile_.is_open() || !logFile_.good()) {
      error("Unable to write log to file: " + file);
      return;
    }
    logFile_.flush();
  }

  info("Log started at: " + Time::Now::getTime());
}

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
                 const std::string& caller, const std::string& trace) {
  addAsyncEvent(
      std::bind(&Logger::addEventLog, this, level, message, caller, trace));
}

void Logger::addEventLog(Level level, const std::string& message,
                         const std::string& caller, const std::string& trace) {
  std::string logMessage;

  if (level == Level::Info && config.showInfo)
    logMessage.append("Info");
  else if (level == Level::Warn && config.showWarning)
    logMessage.append("Warning");
  else if (level == Level::Error && config.showError)
    logMessage.append("Error");
  else if (level == Level::Fatal && config.showFatalError)
    logMessage.append("Fatal error");
  else if (level == Level::Debug && config.showDebug)
    logMessage.append("Debug");
  else
    return;

  if (config.showTime) {
    time_t time = std::time(nullptr);
    tm localTime = *std::localtime(&time);
    std::stringstream stringTime;
    stringTime << std::put_time(&localTime, "%H:%M:%S");
    logMessage.append("[" + stringTime.str() + "]");
  }

  const std::string space = "    ";
  if (caller != "") logMessage.append("\n" + space + "F >> [" + caller + "]");
  if (trace != "") logMessage.append("\n" + space + "T >> [" + trace + "]");

  logMessage.append("\n" + space + "M >> " + message);

  if (config.logToConsole) std::cout << logMessage << std::endl;

  if (config.logToFile) {
    if (logFile_.good()) {
      logFile_ << logMessage << std::endl;
      logFile_.flush();
    }
  }
}
}  // namespace FW::Core
