#include <framework/core/application.h>
#include <framework/global.h>

int main(int argc, char* argv[]) {
  FW::G::Application->start(argc, argv, "Client");
  return 0;
}
