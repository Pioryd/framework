#ifndef FW_CORE_APPLICATIONBASE_H
#define FW_CORE_APPLICATIONBASE_H

#include "declarations.h"

#include "signal.h"
#include <boost/asio.hpp>

namespace FW::Core {
class ApplicationBase {
 public:
  struct Signals {
    FW_CORE_SIGNAL(onInit,
                   void(const std::vector<std::string>&, const std::string&));
    FW_CORE_SIGNAL(onTerminate, void(void));
    // Send signal before application join
    FW_CORE_SIGNAL(onJoin, void(void));

    // When graphical, INDEX[0] is FW::G::Window.poll()
    FW_CORE_SIGNAL(onPollBegin, void(void));
    FW_CORE_SIGNAL(onPollDurring, void(void));
    FW_CORE_SIGNAL(onPollEnd, void(void));
    // Step before while loop start.
    FW_CORE_SIGNAL(onStartMainLoop, void(void));
  };

  enum class Type { Unknown, Console, Graphical };

 protected:
  enum class State {
    Unknown,
    Initiating,
    Initiated,
    Running,
    Terminating,
    Terminated
  };

 public:
  ApplicationBase(Type type);
  virtual ~ApplicationBase() = default;

  void start(int32_t argc, char* argv[], const std::string& title);
  void close();

  boost::asio::io_service& get_io_service();
  std::string getCompileInfo();
  std::filesystem::path getAssetsPath();
  std::wstring getWorkingDirectory();
  Type get_type();

  bool isRunning();

 protected:
  void init(const std::vector<std::string>& argv, const std::string& title);
  void terminate();
  void join();

  void poll();

  virtual void startMainLoop() = 0;

  virtual void onInit(const std::vector<std::string>&,
                      const std::string& title) = 0;
  virtual void onTerminate() = 0;
  virtual void onJoin() = 0;

  static void handleSystemSignal(int32_t signal);
  void asyncWaitForSignal();

  void connect_cms_modules();

 public:
  Signals signals;

  static inline std::vector<void (*)(void)> on_load_vec;
  static inline std::vector<void (*)(void)> on_init_vec;
  static inline std::vector<void (*)(void)> on_terminate_vec;
  static inline std::vector<void (*)(void)> on_reload_vec;

 protected:
  State state_;
  Type type_;
  boost::asio::io_service io_service_;
  boost::asio::signal_set signal_set_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATIONBASE_H
