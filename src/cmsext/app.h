#ifndef FW_CMSEXT_APP_H
#define FW_CMSEXT_APP_H

#include <cmsext/declarations.h>

namespace cmsext::app {
void close();

cms::string get_compile_info();
cms::string get_assets_path();
cms::string get_working_directory();

bool is_running();
}  // namespace cmsext::app
#endif  // #ifndef FW_CMSEXT_APP_H
