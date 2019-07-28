#include "init.h"

#ifdef FW_DEF_GRAPHICAL_APPLICATION
#include "ui.h"
#endif

namespace Modules {
void init(const std::vector<std::string>& argv, const std::string& title) {
#ifdef FW_DEF_GRAPHICAL_APPLICATION
  UI::init(argv, title);
#endif
}
}  // namespace Modules