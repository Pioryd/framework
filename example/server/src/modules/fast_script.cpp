#include <cmsext/cmsext.h>

CMS_BEGIN(fast_script, "")
cms::string text_box_script;

void on_click_run_as_python() {
  run_script(text_box_get_text(text_box_script));
}
void on_click_run_as_command() {
  auto command = text_box_get_text(text_box_script);
  if (command == "exit") {
    cmsext::app::close();
  } else if (command == "--help") {
    info("Commnads list:\n[exit] - exit application" +
         cms::string("\n[--help] - print list of commands"));
  } else {
    error("Command not found: " + command +
          ". Print '--help' for command list.");
  }
}
void create_ui() {
  cms::string root = "root";

  auto label = label_create(root);
  widget_set_position(label, 5, 5);
  label_set_text(label, "Terminal:");
  label_set_text_size(label, 18);

  text_box_script = text_box_create(root);
  widget_set_size(text_box_script, 695, 315);
  widget_set_position(text_box_script, 5, 30);
  text_box_set_text_size(text_box_script, 13);
  text_box_set_text(text_box_script, "Build: " + cms::string(__DATE__) + " " +
                                         cms::string(__TIME__));
  
  auto button_run_as_python = button_create(root);
  widget_set_position(button_run_as_python, 5, 350);
  widget_set_size(button_run_as_python, 345, 40);
  widget_connect(button_run_as_python, "pressed", on_click_run_as_python);
  button_set_text(button_run_as_python, "Run as python script");

  auto button_run_as_command = button_create(root);
  widget_set_position(button_run_as_command, 355, 350);
  widget_set_size(button_run_as_command, 345, 40);
  widget_connect(button_run_as_command, "pressed", on_click_run_as_command);
  button_set_text(button_run_as_command, "Run as command");
}
// Signals
void on_load() {}
void on_init() { create_ui(); }
void on_terminate() {}
void on_reload() {}

CMS_END()
