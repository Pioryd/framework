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
  void start_main_loop() override;

 protected:
  void on_init(const std::vector<std::string>&,
               const std::string& title) override;
  void on_terminate() override;
  void on_join() override;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATION_CONSOLE_H
