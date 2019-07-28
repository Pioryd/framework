#include "serverapp.h"

#include <framework/core/application.h>
#include <framework/core/logger.h>
#include <framework/core/signal.h>
#include <framework/crypt/rsa.h>
#include <framework/global.h>
#include <framework/net/tcplistener.h>
#include <framework/thread/eventmanager.h>

#include "protocolapp.h"

namespace Server {
ServerApp::ServerApp() : state_{State::NONE} {}

void ServerApp::init(const std::vector<std::string>& argv,
                     const std::string& title) {
  state_ = State::INITIALIZING;

  setup();

  if (state_ == State::TERMINATED)
    FW::G::Application->close();
  else
    state_ = State::INITIALIZED;
}

void ServerApp::onTerminate() {
  if (tcpListenerApp_) tcpListenerApp_->stop();
}

void ServerApp::onJoin() {}

void ServerApp::setup() {
  try {
    FW::G::Logger.info(FW::G::Application.force()->getCompileInfo());
    setupSignals();
    setupRsa();
    setupDatabase();
    setupTcpListeners();

    FW::G::Logger.info("Loaded all modules, server starting up...");
  } catch (std::exception& e) {
    FW::G::Logger.error("Exception: " + std::string(e.what()));
    state_ = State::TERMINATED;
  } catch (...) {
    FW::G::Logger.error("Unknown exception.");
    state_ = State::TERMINATED;
  }
}

void ServerApp::setupSignals() {
  FW::G::Application->signals.onTerminate.connect(
      "ServerApp::onTerminate()", &ServerApp::onTerminate, this);
  FW::G::Application->signals.onJoin.connect("ServerApp::onJoin()",
                                             &ServerApp::onJoin, this);
  FW::G::Application->signals.onStartMainLoop.connect(
      "ServerApp::onJoin()",
      [=]() { FW::G::Logger.info(config.appName + " status: [Online]"); });
}

void ServerApp::setupRsa() {
  FW::G::Logger.info("Setup rsa...");
  const char* hex_n =
      "c1094daa17ffa636eec1cd378ee86036757946d1fba8726c1fd6becb48546e6dda7e"
      "3e"
      "a5fb70aa800b92fe3bf56288f0c3d7c184d0f0a0c1f0f19594fb11875c53827ff7ee"
      "4a"
      "20"
      "f0cf5eb5a60b7e2739b5c7239582f409f17e6c58b4483d0ae77c3470f795baf8b90e"
      "72"
      "92"
      "c9e86345c41aed01c25d1a148f7099d17d98c8446dh";
  const char* hex_e = "11h";
  const char* hex_d(
      "0x1bf372572e55ab2e2e27a53e7cf2276a58d42ab05c0a7d746f2e2735c2a7727386"
      "bb"
      "66"
      "e639421fa3563cab2ee87434434bd9bd08ce537e5f447b04488a4a5b6af1c593471c"
      "57"
      "c2"
      "eb31d76324dc996595912dbcde6055b05542511107a4389a9da01416317c5b9416ea"
      "04"
      "46"
      "fb907dd39c3bd5edf406d40b92e04ab7d0d97ee49h");

  FW::G::Rsa.setPrivateKey(hex_n, hex_e, hex_d);
}

void ServerApp::setupDatabase() {}

void ServerApp::setupTcpListeners() {
  tcpListenerApp_ = std::make_shared<FW::Net::TcpListener>(
      FW::G::Application.force()->get_io_service(), ProtocolApp::create,
      config.protocolAppHost, config.protocolAppPort, FW::G::eventManager);

  tcpListenerApp_->start();

  if (!tcpListenerApp_->isRunning())
    throw std::runtime_error("TcpListener [App] is no loaded.");
}
}  // namespace Server
