#include "logger.h"

#include "../time/time.h"

namespace FW::Core {
Logger::Logger(const std::string& traceInfo) : EventManager(traceInfo) {}

void Logger::start(const std::string& file) {
  EventManager::start();

  if (config.log_to_file) {
    log_file_.open(file.c_str(), std::ios::out | std::ios::trunc);
    if (!log_file_.is_open() || !log_file_.good()) {
      error("Unable to write log to file: " + file);
      return;
    }
    log_file_.flush();
  }

  info("Log started at: " + Time::Now::get_time());
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
  add_async_event(
      std::bind(&Logger::addEventLog, this, level, message, caller, trace));
}

void Logger::addEventLog(Level level, const std::string& message,
                         const std::string& caller, const std::string& trace) {
  std::string log_message;

  if (level == Level::Info && config.show_info)
    log_message.append("Info");
  else if (level == Level::Warn && config.show_warning)
    log_message.append("Warning");
  else if (level == Level::Error && config.show_error)
    log_message.append("Error");
  else if (level == Level::Fatal && config.show_fatal_error)
    log_message.append("Fatal error");
  else if (level == Level::Debug && config.show_debug)
    log_message.append("Debug");
  else
    return;

  if (config.show_time) {
    time_t time = std::time(nullptr);
    tm local_time = *std::localtime(&time);
    std::stringstream string_time;
    string_time << std::put_time(&local_time, "%H:%M:%S");
    log_message.append("[" + string_time.str() + "]");
  }

  const std::string space = "    ";
  if (caller != "") log_message.append("\n" + space + "F >> [" + caller + "]");
  if (trace != "") log_message.append("\n" + space + "T >> [" + trace + "]");

  log_message.append("\n" + space + "M >> " + message);

  if (config.log_to_console) std::cout << log_message << std::endl;

  if (config.log_to_file) {
    if (log_file_.good()) {
      log_file_ << log_message << std::endl;
      log_file_.flush();
    }
  }
}
}  // namespace FW::Core
