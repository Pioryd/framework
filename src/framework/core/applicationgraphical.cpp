#include "applicationgraphical.h"

#include <framework/multimedia/soundmanager.h>
#include <framework/multimedia/window.h>
#include <framework/thread/eventmanager.h>
#include <framework/time/clock.h>

namespace FW::Core {
ApplicationGraphical::ApplicationGraphical() : ApplicationBase() {
  // Inside constructor to be sure that will be called first, before user
  // instructions
  signals.onInit.connect("ApplicationGraphical::onInit",
                         &ApplicationGraphical::onInit, this);
  signals.onTerminate.connect("ApplicationGraphical::onTerminate",
                              &ApplicationGraphical::onTerminate, this);
  signals.onJoin.connect("ApplicationGraphical::onJoin",
                         &ApplicationGraphical::onJoin, this);
  signals.onPollBegin.connect("FW::G::Window.poll", &FW::MM::Window::poll,
                              &FW::G::Window);

  signals.onPollDurring.connect("[this]() { io_service_.poll(); } <1>",
                            [this]() { io_service_.poll(); });

  signals.onPollEnd.connect("[this]() { io_service_.poll(); } <2>",
                            [this]() { io_service_.poll(); });
}

ApplicationGraphical::~ApplicationGraphical() {
  signals.onInit.disconnect("ApplicationGraphical::onInit");
  signals.onTerminate.disconnect("ApplicationGraphical::onTerminate");
  signals.onJoin.disconnect("ApplicationGraphical::onJoin");
  signals.onPollBegin.disconnect("FW::G::Window.poll");
  signals.onPollEnd.disconnect("[this]() { io_service_.poll(); } <1>");
  signals.onPollEnd.disconnect("[this]() { io_service_.poll(); } <2>");
}

void ApplicationGraphical::startMainLoop() {
  FW::G::Clock.update();
  poll();

  FW::G::Clock.update();
  signals.onStartMainLoop.send();

  while (state_ == State::Running && FW::G::Window.isOpen()) {
    FW::G::Clock.update();
    poll();

    FW::G::Window.clear(sf::Color::Yellow);

    FW::G::Window.gui->draw(); 

    FW::G::Window.display();
  }
}

void ApplicationGraphical::onInit(const std::vector<std::string>&,
                                  const std::string& title) {
  FW::G::Window.init({800, 600}, title, false, 60);
}

void ApplicationGraphical::onTerminate() {
}

void ApplicationGraphical::onJoin() {  }
}  // namespace  FW::Core
