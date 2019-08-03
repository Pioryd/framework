#include <framework/global.h>
#include <framework/core/application.h>
#include <framework/pymodule/manager.h>

int main(int argc, char* argv[]) {
  FW::G::Application->start(argc, argv, "Server");
  return 0;
}
