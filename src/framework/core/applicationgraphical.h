#ifndef FW_CORE_APPLICATION_GRAPHICAL_H
#define FW_CORE_APPLICATION_GRAPHICAL_H

#include "declarations.h"

#include "applicationbase.h"

namespace FW::Core {
class ApplicationGraphical : public ApplicationBase {
 public:
  ApplicationGraphical();
  ~ApplicationGraphical();

 protected:
  void startMainLoop() override;

 protected:
  void onInit(const std::vector<std::string>&,
              const std::string& title) override;
  void onTerminate() override;
  void onJoin() override;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATION_GRAPHICAL_H
