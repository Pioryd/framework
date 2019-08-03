#ifndef FW_CMSEXT_GUI_H
#define FW_CMSEXT_GUI_H

#include <cmsext/declarations.h>

namespace cmsext::gui {
void set_window_size(int32_t width, int32_t height);
int32_t get_window_width();
int32_t get_window_height();
}  // namespace cmsext::gui
#endif  // #ifndef FW_CMSEXT_GUI_H
