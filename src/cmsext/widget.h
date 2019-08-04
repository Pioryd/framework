#ifndef FW_CMSEXT_WIDGET_H
#define FW_CMSEXT_WIDGET_H

#include <cmsext/declarations.h>

#include <framework/multimedia/window.h>

struct WidgetEx;

namespace cmsext::widget {

// Widget
void widget_set_position(const WidgetEx& widget, int x, int y);
void widget_set_size(const WidgetEx& widget, int width, int height);
void widget_set_size_ex(const WidgetEx& widget, const cms::string& expression);
void widget_connect(const WidgetEx& widget, const cms::string& signal,
                    std::function<void(void)> function);
// Picture
cms::string picture_create(const WidgetEx& parent, const cms::string& path);
// Tabs
cms::string tabs_create(const WidgetEx& parent);
void tabs_set_tab_height(const WidgetEx& widget, int height);
void tabs_add_tab(const WidgetEx& widget, const cms::string& name);
// MenuBar
cms::string menu_bar_create(const WidgetEx& parent);
void menu_bar_add_menu(const WidgetEx& widget, const cms::string& name);
void menu_bar_add_menu_item(const WidgetEx& widget, const cms::string& name);
// Label
cms::string label_create(const WidgetEx& parent);
void label_set_text(const WidgetEx& widget, const cms::string& text);
void label_set_text_size(const WidgetEx& widget, int size);
// RadioButton
cms::string radio_button_create(const WidgetEx& parent);
void radio_button_set_text(const WidgetEx& widget, const cms::string& text);
// EditBox
cms::string edit_box_create(const WidgetEx& parent);
void edit_box_set_text_size(const WidgetEx& widget, int size);
void edit_box_set_default_text(const WidgetEx& widget, const cms::string& text);
// TextBox
cms::string text_box_create(const WidgetEx& parent);
void text_box_set_text_size(const WidgetEx& widget, int size);
cms::string text_box_get_text(const WidgetEx& widget);
void text_box_set_text(const WidgetEx& widget, const cms::string& text);
void text_box_set_read_only(const WidgetEx& widget, bool read_only);
// ListBox
cms::string list_box_create(const WidgetEx& parent);
void list_box_set_item_height(const WidgetEx& widget, int height);
void list_box_add_item(const WidgetEx& widget, const cms::string& name);
// ProgressBar
cms::string progress_bar_create(const WidgetEx& parent);
void progress_bar_set_value(const WidgetEx& widget, int value);
int progress_bar_get_value(const WidgetEx& widget);
// Slider
cms::string slider_create(const WidgetEx& parent);
void slider_set_value(const WidgetEx& widget, int value);
// Scrollbar
cms::string scrollbar_create(const WidgetEx& parent);
void scrollbar_create_set_maximum(const WidgetEx& widget, int size);
void scrollbar_create_set_viewport_size(const WidgetEx& widget, int size);
// ComboBox
cms::string combo_box_create(const WidgetEx& parent);
void combo_box_add_item(const WidgetEx& widget, const cms::string& name);
void combo_box_set_selected_item(const WidgetEx& widget,
                                 const cms::string& name);
// ChildWindow
cms::string child_window_create(const WidgetEx& parent);
void child_window_set_title(const WidgetEx& widget, const cms::string& name);
// Button
cms::string button_create(const WidgetEx& parent);
void button_set_text(const WidgetEx& widget, const cms::string& text);
// CheckBox
cms::string check_box_create(const WidgetEx& parent);
void check_box_set_text(const WidgetEx& widget, const cms::string& text);
// ChatBox
cms::string chat_box_create(const WidgetEx& parent);
void chat_box_set_text_size(const WidgetEx& widget, int size);
void chat_box_set_lines_start_from_top(const WidgetEx& widget);
void chat_box_add_line(const WidgetEx& widget, const cms::string& text, int r,
                       int g, int b, int a);
// Canvas
cms::string canvas_create(const WidgetEx& parent);
void canvas_clear(const WidgetEx& widget);
void canvas_draw_sprite(const WidgetEx& widget, const cms::string& path,
                        float width_scale, float height_scale);
void canvas_draw_text(const WidgetEx& widget, const cms::string& text,
                      int32_t x, int32_t y, uint32_t size, uint8_t r, uint8_t g,
                      uint8_t b);
void canvas_display(const WidgetEx& widget);
}  // namespace cmsext::widget

struct WidgetEx {
  WidgetEx(tgui::Widget::Ptr& widget);
  WidgetEx(const tgui::Widget::Ptr& widget);

  WidgetEx(cms::string& id);
  WidgetEx(const cms::string& id);

  WidgetEx(const char* id);

  operator tgui::Widget::Ptr&();

  tgui::Widget* operator->() const;

  const tgui::Widget::Ptr& getWidgetPtr() const;

 protected:
  void getWidget(const cms::string& id);
  tgui::Widget::Ptr findWidget(const tgui::Widget::Ptr& widget,
                               const cms::string& id);
  tgui::Widget::Ptr widget_;
};

inline bool operator==(const WidgetEx& lhs, const tgui::Widget::Ptr& rhs) {
  return lhs.getWidgetPtr() == rhs;
}
inline bool operator==(const tgui::Widget::Ptr& lhs, const WidgetEx& rhs) {
  return lhs == rhs.getWidgetPtr();
}
inline bool operator!=(const WidgetEx& lhs, const tgui::Widget::Ptr& rhs) {
  return lhs.getWidgetPtr() != rhs;
}
inline bool operator!=(const tgui::Widget::Ptr& lhs, const WidgetEx& rhs) {
  return lhs != rhs.getWidgetPtr();
}
#endif  // #ifndef FW_CMSEXT_WIDGET_H
