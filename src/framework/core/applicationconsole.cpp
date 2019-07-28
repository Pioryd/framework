#include "applicationconsole.h"

#include <framework/thread/eventmanager.h>
#include <framework/time/clock.h>

namespace FW::Core {
ApplicationConsole::ApplicationConsole() : ApplicationBase() {
  // Inside constructor to be sure that will be called first
  signals.onInit.connect("ApplicationConsole::onInit",
                         &ApplicationConsole::onInit, this);
  signals.onTerminate.connect("ApplicationConsole::onTerminate",
                              &ApplicationConsole::onTerminate, this);
  signals.onJoin.connect("ApplicationConsole::onJoin",
                         &ApplicationConsole::onJoin, this);
  signals.onPollEnd.connect("[this]() { io_service_.poll(); }",
                            [this]() { io_service_.poll(); });
}

ApplicationConsole::~ApplicationConsole() {
  signals.onInit.disconnect("ApplicationConsole::onInit");
  signals.onInit.disconnect("ApplicationConsole::onTerminate");
  signals.onJoin.disconnect("ApplicationConsole::onJoin");
  signals.onPollEnd.disconnect("[this]() { io_service_.poll(); }");
}

void ApplicationConsole::startMainLoop() {
  FW::G::Clock.update();
  poll();

  FW::G::Clock.update();
  signals.onStartMainLoop.send();

  while (state_ == State::Running) {
    FW::G::Clock.update();
    poll();
  }
}

void ApplicationConsole::onInit(const std::vector<std::string>&,
                                const std::string& title) {
#ifdef _WIN32
  SetConsoleTitle(title.c_str());
#endif
}

void ApplicationConsole::onTerminate() {}

void ApplicationConsole::onJoin() {}
}  // namespace  FW::Core
