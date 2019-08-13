#include "applicationconsole.h"

#include <framework/thread/eventmanager.h>
#include <framework/time/clock.h>

namespace FW::Core {
ApplicationConsole::ApplicationConsole() : ApplicationBase(Type::Console) {
  // Inside constructor to be sure that will be called first
  signals.on_init.connect("ApplicationConsole::on_init",
                         &ApplicationConsole::on_init, this);
  signals.on_terminate.connect("ApplicationConsole::on_terminate",
                              &ApplicationConsole::on_terminate, this);
  signals.on_join.connect("ApplicationConsole::on_join",
                         &ApplicationConsole::on_join, this);
  signals.on_poll_end.connect("[this]() { io_service_.poll(); }",
                            [this]() { io_service_.poll(); });
}

ApplicationConsole::~ApplicationConsole() {
  signals.on_init.disconnect("ApplicationConsole::on_init");
  signals.on_init.disconnect("ApplicationConsole::on_terminate");
  signals.on_join.disconnect("ApplicationConsole::on_join");
  signals.on_poll_end.disconnect("[this]() { io_service_.poll(); }");
}

void ApplicationConsole::start_main_loop() {
  FW::G::Clock.update();
  poll();

  FW::G::Clock.update();
  signals.on_start_main_loop.send();

  while (state_ == State::Running) {
    FW::G::Clock.update();
    poll();
  }
}

void ApplicationConsole::on_init(const std::vector<std::string>&,
                                const std::string& title) {
#ifdef _WIN32
  SetConsoleTitle(title.c_str());
#endif
}

void ApplicationConsole::on_terminate() {}

void ApplicationConsole::on_join() {}
}  // namespace  FW::Core
