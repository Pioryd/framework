#include "applicationbase.h"

#include "application.h"
#include "logger.h"

#include "../global.h"
#include "../platform/platform.h"
#include "../thread/eventmanager.h"
#include "../util/file.h"

void std_handler() {
  std::cout << "Out of memory." << std::endl;
  exit(-1);
}

namespace FW::Core {
ApplicationBase::ApplicationBase()
    : state_{State::Unknown}, signal_set_(io_service_) {
  signal_set_.add(SIGINT);
  signal_set_.add(SIGTERM);
#ifdef _WIN32
  signal(SIGBREAK, &ApplicationBase::handleSystemSignal);
#endif  // #ifdef _WIN32

  asyncWaitForSignal();
}

void ApplicationBase::start(int32_t argc, char* argv[],
                            const std::string& title) {
  std::vector<std::string> argvVector;
  for (int32_t i = 0; i < argc; i++) argvVector.push_back(argv[i]);

  init(argvVector, title);

  if (state_ == State::Initiated) {
    state_ = State::Running;
    startMainLoop();
  }

  terminate();
  join();
}

void ApplicationBase::close() { terminate(); }

boost::asio::io_service& ApplicationBase::get_io_service() {
  return io_service_;
}

std::string ApplicationBase::getCompileInfo() {
  return "Compile info:\n\tCompiler: " + std::string(BOOST_COMPILER) +
         "\n\tTime: " + std::string(__DATE__) + ' ' + __TIME__ +
         "\n\tPlatform: " + FW::Platform::name() + "\n\tArchitecture: " +
#if defined(BOOST_ARCH_X86)
#if BOOST_ARCH_X86_64
         "x86_64 ";
#elif BOOST_ARCH_X86_32
         "x86 ";
#endif  // #if BOOST_ARCH_X86_64
#elif defined(BOOST_ARCH_ARM)
#if _M_ARM
         "ARM ";
#elif _M_ARM64
             std::cout
         << "ARM64 " << _M_ARM64 << " \n";
#else
         " Unknown"
#endif  // #if _M_ARM
#else
         " Unknown"
#endif  // #if defined(BOOST_ARCH_X86)
}

std::filesystem::path ApplicationBase::getAssetsPath() {
  std::filesystem::path assetsPath;

  if (FW_DEF_PATH_ASSETS == "assets")
    assetsPath.append(getWorkingDirectory());
  
  assetsPath.append(FW_DEF_PATH_ASSETS);

  return assetsPath;
}

std::wstring ApplicationBase::getWorkingDirectory() {
  std::wstring workingDirectory;
#ifdef FW_DEF_PATH_WORK
#define WSTRINGIFY(x) L##x
#define TOWSTRING(x) WSTRINGIFY(x)
  workingDirectory = TOWSTRING(FW_DEF_PATH_WORK);
#else   // #ifdef FW_DEF_PATH_WORK
  workingDirectory = IO::File::getExecutableDirectory();
#endif  // #ifdef FW_DEF_PATH_WORK
  return workingDirectory;
}

bool ApplicationBase::isRunning() { return (state_ == State::Running); }

void ApplicationBase::init(const std::vector<std::string>& argv,
                           const std::string& title) {
  state_ = State::Initiating;
  std::set_new_handler(std_handler);

  FW::G::Logger.start("log.txt");
  FW::G::PyModuleManager->init();

  FW::G::eventManager.start();

  signals.onInit.send(argv, title);

  if (state_ == State::Initiating) state_ = State::Initiated;
}

void ApplicationBase::terminate() {
  state_ = State::Terminating;
  signals.onTerminate.send();

  FW::G::PyModuleManager->terminate();
  FW::G::eventManager.terminate();
  FW::G::Logger.terminate();

  state_ = State::Terminated;
}

void ApplicationBase::join() {
  signals.onJoin.send();

  FW::G::eventManager.join();
  FW::G::Logger.join();
}

void ApplicationBase::poll() {
  signals.onPollBegin.send();
  signals.onPollDurring.send();
  signals.onPollEnd.send();
}

void ApplicationBase::handleSystemSignal(int32_t signal) {
  if (signal == SIGINT)
    G::eventManager.addAsyncEvent([]() {
      FW::G::Application->terminate();
      FW::G::Application->join();
    });
  else if (signal == SIGTERM)
    G::eventManager.addAsyncEvent([]() {
      FW::G::Application->terminate();
      FW::G::Application->join();
    });
#ifdef _WIN32
  else if (signal == SIGBREAK)
    G::eventManager.addAsyncEvent([]() {
      FW::G::Application->terminate();
      FW::G::Application->join();
    });
#endif
}

void ApplicationBase::asyncWaitForSignal() {
  signal_set_.async_wait([this](boost::system::error_code ec, int32_t signal) {
    if (ec) {
      std::cerr << "Unable to handle system signal. Error: " << ec.message()
                << std::endl;
    } else {
      std::cerr << "Recived system signal: " << signal << std::endl;
      ApplicationBase::handleSystemSignal(signal);
      asyncWaitForSignal();
    }
  });
}
}  // namespace  FW::Core
