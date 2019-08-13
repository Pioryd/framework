#ifndef FW_CORE_APPLICATIONBASE_H
#define FW_CORE_APPLICATIONBASE_H

#include "declarations.h"

#include "signal.h"
#include <boost/asio.hpp>

namespace FW::Core {
class ApplicationBase {
 public:
  struct Signals {
    FW_CORE_SIGNAL(on_init,
                   void(const std::vector<std::string>&, const std::string&));
    FW_CORE_SIGNAL(on_terminate, void(void));
    // Send signal before application join
    FW_CORE_SIGNAL(on_join, void(void));

    // When graphical, INDEX[0] is FW::G::Window.poll()
    FW_CORE_SIGNAL(on_poll_begin, void(void));
    FW_CORE_SIGNAL(on_poll_durring, void(void));
    FW_CORE_SIGNAL(on_poll_end, void(void));
    // Step before while loop start.
    FW_CORE_SIGNAL(on_start_main_loop, void(void));
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
  std::string get_compile_info();
  std::filesystem::path get_assets_path();
  std::wstring get_working_directory();
  Type get_type();

  bool is_running();

 protected:
  void init(const std::vector<std::string>& argv, const std::string& title);
  void terminate();
  void join();

  void poll();

  virtual void start_main_loop() = 0;

  virtual void on_init(const std::vector<std::string>&,
                      const std::string& title) = 0;
  virtual void on_terminate() = 0;
  virtual void on_join() = 0;

  static void handleSystemSignal(int32_t signal);
  void async_wait_for_signal();

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
