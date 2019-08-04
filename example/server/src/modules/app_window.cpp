#include <cmsext/app.h>
#include <cmsext/gui.h>
#include <cmsext/log.h>
#include <cmsext/widget.h>

using namespace cmsext::widget;
using namespace cmsext::app;
using namespace cmsext::gui;
using namespace cmsext::log;

CMS_BEGIN(app_window, "try_run_first")

void create_ui() {
  set_window_size(800, 400);

  cms::string root = "root";

  auto background =
      picture_create(root, get_assets_path() + "/image/white.jpg");
  widget_set_size_ex(background, "100%, 100%"); 
}

void on_load() {}
void on_init() { create_ui(); }
void on_terminate() {}
void on_reload() {}
CMS_END()
