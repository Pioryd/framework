#include <framework/core/application.h>
#include <framework/global.h>

#include "clientapp.h"
#include "global.h"

#include <modules/init.h>

int main(int argc, char* argv[]) {
  FW::G::Application->signals.onInit.connect(
      "Server::G::serverApp", &Client::ClientApp::init, &Client::G::clientApp);
  FW::G::Application->signals.onInit.connect("Modules::init", &Modules::init);

  FW::G::Application->start(argc, argv, Client::G::clientApp.config.appName);
  return 0;
}
