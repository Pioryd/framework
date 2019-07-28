#ifndef FW_CORE_APPLICATION_CONSOLE_H
#define FW_CORE_APPLICATION_CONSOLE_H

#include "declarations.h"

#include "applicationbase.h"

namespace FW::Core {
class ApplicationConsole : public ApplicationBase {
 public:
  ApplicationConsole();
  ~ApplicationConsole();

 protected:
  void startMainLoop() override;

 protected:
  void onInit(const std::vector<std::string>&,
              const std::string& title) override;
  void onTerminate() override;
  void onJoin() override;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATION_CONSOLE_H
