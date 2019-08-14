#include "applicationgraphical.h"

#include <framework/multimedia/soundmanager.h>
#include <framework/multimedia/window.h>
#include <framework/thread/eventmanager.h>
#include <framework/time/clock.h>

namespace FW::Core {
ApplicationGraphical::ApplicationGraphical()
    : ApplicationBase(Type::Graphical) {
  // Inside constructor to be sure that will be called first, before user
  // instructions
  signals.on_init.connect("ApplicationGraphical::on_init",
                         &ApplicationGraphical::on_init, this);
  signals.on_terminate.connect("ApplicationGraphical::on_terminate",
                              &ApplicationGraphical::on_terminate, this);
  signals.on_join.connect("ApplicationGraphical::on_join",
                         &ApplicationGraphical::on_join, this);
  signals.on_poll_begin.connect("FW::G::Window.poll", &FW::MM::Window::poll,
                              &FW::G::Window);

  signals.on_poll_durring.connect("[this]() { io_service_.poll(); } <1>",
                            [this]() { io_service_.poll(); });

  signals.on_poll_end.connect("[this]() { io_service_.poll(); } <2>",
                            [this]() { io_service_.poll(); });
}

ApplicationGraphical::~ApplicationGraphical() {
  signals.on_init.disconnect("ApplicationGraphical::on_init");
  signals.on_terminate.disconnect("ApplicationGraphical::on_terminate");
  signals.on_join.disconnect("ApplicationGraphical::on_join");
  signals.on_poll_begin.disconnect("FW::G::Window.poll");
  signals.on_poll_end.disconnect("[this]() { io_service_.poll(); } <1>");
  signals.on_poll_end.disconnect("[this]() { io_service_.poll(); } <2>");
}

void ApplicationGraphical::start_main_loop() {
  FW::G::Clock.update();
  poll();

  FW::G::Clock.update();
  signals.on_start_main_loop.send();

  while (state_ == State::Running && FW::G::Window.is_open()) {
    FW::G::Clock.update();
    poll();

    FW::G::Window.clear(sf::Color::Yellow);

    FW::G::Window.gui->draw(); 

    FW::G::Window.display();
  }
}

void ApplicationGraphical::on_init(const std::vector<std::string>&,
                                  const std::string& title) {
  FW::G::Window.init({800, 600}, title, false, 60);
}

void ApplicationGraphical::on_terminate() {
}

void ApplicationGraphical::on_join() {  }
}  // namespace  FW::Core
