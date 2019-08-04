#include "widget.h"

#include <framework/core/logger.h>
#include <framework/global.h>
#include <framework/multimedia/window.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>
struct AutoId {
  // TODO
  // Its not safe. Only for time untile rework widgets.
  std::string get_id() {
    number++;
    if (number == 0) additional_string += "x";
    return std::to_string(number) + additional_string;
  }

 protected:
  std::string additional_string;
  int32_t number = 0;
};
static inline AutoId AUTO_ID;

namespace cmsext::widget {
//////////////////////////////// Widget ////////////////////////////////////////
void widget_set_position(const WidgetEx& widget, int x, int y) {
  if (!widget.getWidgetPtr()) return;
  widget.getWidgetPtr()->setPosition(x, y);
}

void widget_set_size(const WidgetEx& widget, int width, int height) {
  if (!widget.getWidgetPtr()) return;
  widget.getWidgetPtr()->setSize(width, height);
}

void widget_set_size_ex(const WidgetEx& widget, const cms::string& expression) {
  if (!widget.getWidgetPtr()) return;
  widget.getWidgetPtr()->setSize((std::string)expression);
}

void widget_connect(const WidgetEx& widget, const cms::string& signal,
                    std::function<void(void)> function) {
  if (!widget.getWidgetPtr()) return;
  widget.getWidgetPtr()->connect(signal, function);
}
////////////////////////////// Picture /////////////////////////////////////////
cms::string picture_create(const WidgetEx& parent, const cms::string& path) {
  auto widget = tgui::Picture::create((std::string)path);
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);

  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}
//////////////////////////////// Tabs //////////////////////////////////////////
cms::string tabs_create(const WidgetEx& parent) {
  auto widget = tgui::Tabs::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("Tabs"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void tabs_set_tab_height(const WidgetEx& widget, int height) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Tabs") return;
  widget.getWidgetPtr()->cast<tgui::Tabs>()->setTabHeight(30);
}

void tabs_add_tab(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Tabs") return;
  widget.getWidgetPtr()->cast<tgui::Tabs>()->add((std::string)name);
}
////////////////////////////// Menu Bar ////////////////////////////////////////
cms::string menu_bar_create(const WidgetEx& parent) {
  auto widget = tgui::MenuBar::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("MenuBar"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void menu_bar_add_menu(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "MenuBar") return;
  widget.getWidgetPtr()->cast<tgui::MenuBar>()->addMenu((std::string)name);
}

void menu_bar_add_menu_item(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "MenuBar") return;
  widget.getWidgetPtr()->cast<tgui::MenuBar>()->addMenuItem((std::string)name);
}
/////////////////////////////// Label //////////////////////////////////////////
cms::string label_create(const WidgetEx& parent) {
  auto widget = tgui::Label::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("Label"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void label_set_text(const WidgetEx& widget, const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Label") return;
  widget.getWidgetPtr()->cast<tgui::Label>()->setText((std::string)text);
}

void label_set_text_size(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Label") return;
  widget.getWidgetPtr()->cast<tgui::Label>()->setTextSize(size);
}
//////////////////////////// Radio Button //////////////////////////////////////
cms::string radio_button_create(const WidgetEx& parent) {
  auto widget = tgui::RadioButton::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("RadioButton"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void radio_button_set_text(const WidgetEx& widget, const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "RadioButton")
    return;
  widget.getWidgetPtr()->cast<tgui::RadioButton>()->setText((std::string)text);
}
////////////////////////////// Edit Box ////////////////////////////////////////
cms::string edit_box_create(const WidgetEx& parent) {
  auto widget = tgui::EditBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("EditBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void edit_box_set_text_size(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "EditBox") return;
  widget.getWidgetPtr()->cast<tgui::EditBox>()->setTextSize(size);
}

void edit_box_set_default_text(const WidgetEx& widget,
                               const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "EditBox") return;
  widget.getWidgetPtr()->cast<tgui::EditBox>()->setDefaultText(
      (std::string)text);
}
////////////////////////////// Text Box ////////////////////////////////////////
cms::string text_box_create(const WidgetEx& parent) {
  auto widget = tgui::TextBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("TextBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void text_box_set_text_size(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "TextBox") return;
  widget.getWidgetPtr()->cast<tgui::TextBox>()->setTextSize(size);
}

cms::string text_box_get_text(const WidgetEx& widget) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "TextBox") return "";
  return widget.getWidgetPtr()->cast<tgui::TextBox>()->getText();
}

void text_box_set_text(const WidgetEx& widget, const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "TextBox") return;
  return widget.getWidgetPtr()->cast<tgui::TextBox>()->setText(
      (std::string)text);
}

void text_box_set_read_only(const WidgetEx& widget, bool read_only) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "TextBox") return;
  widget.getWidgetPtr()->cast<tgui::TextBox>()->setReadOnly(read_only);
}
////////////////////////////// List Box ////////////////////////////////////////
cms::string list_box_create(const WidgetEx& parent) {
  auto widget = tgui::ListBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ListBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void list_box_set_item_height(const WidgetEx& widget, int height) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ListBox") return;
  widget.getWidgetPtr()->cast<tgui::ListBox>()->setItemHeight(height);
}

void list_box_add_item(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ListBox") return;
  widget.getWidgetPtr()->cast<tgui::ListBox>()->addItem((std::string)name);
}
//////////////////////////// Progress Bar //////////////////////////////////////
cms::string progress_bar_create(const WidgetEx& parent) {
  auto widget = tgui::ProgressBar::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ProgressBar"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void progress_bar_set_value(const WidgetEx& widget, int value) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ProgressBar")
    return;
  widget.getWidgetPtr()->cast<tgui::ProgressBar>()->setValue(value);
}

int progress_bar_get_value(const WidgetEx& widget) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ProgressBar")
    return -1;
  return widget.getWidgetPtr()->cast<tgui::ProgressBar>()->getValue();
}
////////////////////////////// Slider //////////////////////////////////////////
cms::string slider_create(const WidgetEx& parent) {
  auto widget = tgui::Slider::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("Slider"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void slider_set_value(const WidgetEx& widget, int value) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Slider") return;
  widget.getWidgetPtr()->cast<tgui::Slider>()->setValue(value);
}
//////////////////////////// ScrollBar /////////////////////////////////////////
cms::string scrollbar_create(const WidgetEx& parent) {
  auto widget = tgui::Scrollbar::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ScrollBar"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void scrollbar_create_set_maximum(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Scrollbar") return;
  widget.getWidgetPtr()->cast<tgui::Scrollbar>()->setMaximum(size);
}

void scrollbar_create_set_viewport_size(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Scrollbar") return;
  widget.getWidgetPtr()->cast<tgui::Scrollbar>()->setViewportSize(size);
}
///////////////////////////// ComboBox /////////////////////////////////////////
cms::string combo_box_create(const WidgetEx& parent) {
  auto widget = tgui::ComboBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ComboBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void combo_box_add_item(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ComboBox") return;
  widget.getWidgetPtr()->cast<tgui::ComboBox>()->addItem((std::string)name);
}

void combo_box_set_selected_item(const WidgetEx& widget,
                                 const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ComboBox") return;
  widget.getWidgetPtr()->cast<tgui::ComboBox>()->setSelectedItem(
      (std::string)name);
}
///////////////////////////// ChildWindow //////////////////////////////////////
cms::string child_window_create(const WidgetEx& parent) {
  auto widget = tgui::ChildWindow::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ChildWindow"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void child_window_set_title(const WidgetEx& widget, const cms::string& name) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ChildWindow")
    return;
  widget.getWidgetPtr()->cast<tgui::ChildWindow>()->setTitle((std::string)name);
}
//////////////////////////////// Button ////////////////////////////////////////
cms::string button_create(const WidgetEx& parent) {
  auto widget = tgui::Button::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("Button"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void button_set_text(const WidgetEx& widget, const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "Button") return;
  widget.getWidgetPtr()->cast<tgui::Button>()->setText((std::string)text);
}
/////////////////////////////// CheckBox ///////////////////////////////////////
cms::string check_box_create(const WidgetEx& parent) {
  auto widget = tgui::CheckBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("CheckBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void check_box_set_text(const WidgetEx& widget, const cms::string& text) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "CheckBox") return;
  widget.getWidgetPtr()->cast<tgui::CheckBox>()->setText((std::string)text);
}
/////////////////////////////// ChatBox ///////////////////////////////////////
cms::string chat_box_create(const WidgetEx& parent) {
  auto widget = tgui::ChatBox::create();
  widget->setRenderer(FW::G::Window.theme.getRenderer("ChatBox"));
  if (parent != nullptr && parent->getWidgetType() == "ChildWindow")
    parent->cast<tgui::ChildWindow>()->add(widget);
  else
    FW::G::Window.gui->add(widget);
  cms::string id(AUTO_ID.get_id());
  widget->setUserData(id);
  return id;
}

void chat_box_set_text_size(const WidgetEx& widget, int size) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ChatBox") return;
  widget.getWidgetPtr()->cast<tgui::ChatBox>()->setTextSize(size);
}

void chat_box_set_lines_start_from_top(const WidgetEx& widget) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ChatBox") return;
  widget.getWidgetPtr()->cast<tgui::ChatBox>()->setLinesStartFromTop();
}

void chat_box_add_line(const WidgetEx& widget, const cms::string& text, int r,
                       int g, int b, int a) {
  if (!widget.getWidgetPtr() || widget->getWidgetType() != "ChatBox") return;
  widget.getWidgetPtr()->cast<tgui::ChatBox>()->addLine((std::string)text,
                                                        sf::Color(r, g, b, a));
}
}  // namespace cmsext::widget

PYBIND11_EMBEDDED_MODULE(cmsext_widget, m) {
  // Widget
  m.def("widget_set_position", cmsext::widget::widget_set_position),  //
      m.def("widget_set_size", cmsext::widget::widget_set_size),
      m.def("widget_set_size_ex", cmsext::widget::widget_set_size_ex),
      m.def("widget_connect", cmsext::widget::widget_connect),
      // Picture
      m.def("picture_create", cmsext::widget::picture_create),
      // Tabs
      m.def("tabs_create", cmsext::widget::tabs_create),
      m.def("tabs_set_tab_height", cmsext::widget::tabs_set_tab_height),
      m.def("tabs_add_tab", cmsext::widget::tabs_add_tab),
      // MenuBar
      m.def("menu_bar_create", cmsext::widget::menu_bar_create),
      m.def("menu_bar_add_menu", cmsext::widget::menu_bar_add_menu),
      m.def("menu_bar_add_menu_item", cmsext::widget::menu_bar_add_menu_item),
      // Label
      m.def("label_create", cmsext::widget::label_create),
      m.def("label_set_text", cmsext::widget::label_set_text),
      m.def("label_set_text_size", cmsext::widget::label_set_text_size),
      // RadioButton
      m.def("radio_button_create", cmsext::widget::radio_button_create),
      m.def("radio_button_set_text", cmsext::widget::radio_button_set_text),
      // EditBox
      m.def("edit_box_create", cmsext::widget::edit_box_create),
      m.def("edit_box_set_text_size", cmsext::widget::edit_box_set_text_size),
      m.def("edit_box_set_default_text",
            cmsext::widget::edit_box_set_default_text),
      // ListBox
      m.def("list_box_create", cmsext::widget::list_box_create),
      m.def("list_box_set_item_height",
            cmsext::widget::list_box_set_item_height),
      m.def("list_box_add_item", cmsext::widget::list_box_add_item),
      // ProgressBar
      m.def("progress_bar_create", cmsext::widget::progress_bar_create),
      m.def("progress_bar_set_value", cmsext::widget::progress_bar_set_value),
      m.def("progress_bar_get_value", cmsext::widget::progress_bar_get_value),
      // Slider
      m.def("slider_create", cmsext::widget::slider_create),
      m.def("slider_set_value", cmsext::widget::slider_set_value),
      // Scrollbar
      m.def("scrollbar_create", cmsext::widget::scrollbar_create),
      m.def("scrollbar_create_set_maximum",
            cmsext::widget::scrollbar_create_set_maximum),
      m.def("scrollbar_create_set_viewport_size",
            cmsext::widget::scrollbar_create_set_viewport_size),
      // ComboBox
      m.def("combo_box_create", cmsext::widget::combo_box_create),
      m.def("combo_box_add_item", cmsext::widget::combo_box_add_item),
      m.def("combo_box_set_selected_item",
            cmsext::widget::combo_box_set_selected_item),
      // ChildWindow
      m.def("child_window_create", cmsext::widget::child_window_create),
      m.def("child_window_set_title", cmsext::widget::child_window_set_title),
      // Button
      m.def("button_create", cmsext::widget::button_create),
      m.def("button_set_text", cmsext::widget::button_set_text),
      // CheckBox
      m.def("check_box_create", cmsext::widget::check_box_create),
      m.def("check_box_set_text", cmsext::widget::check_box_set_text),
      // ChatBox
      m.def("chat_box_create", cmsext::widget::chat_box_create),
      m.def("chat_box_set_text_size", cmsext::widget::chat_box_set_text_size),
      m.def("chat_box_set_lines_start_from_top",
            cmsext::widget::chat_box_set_lines_start_from_top),
      m.def("chat_box_add_line", cmsext::widget::chat_box_add_line);
}

WidgetEx::WidgetEx(tgui::Widget::Ptr& widget) { widget_ = widget; }

WidgetEx::WidgetEx(const tgui::Widget::Ptr& widget) { widget_ = widget; }

WidgetEx::WidgetEx(cms::string& id) { getWidget(id); }

WidgetEx::WidgetEx(const cms::string& id) { getWidget(id); }

WidgetEx::WidgetEx(const char* id) { getWidget(id); }

WidgetEx::operator tgui::Widget::Ptr&() { return widget_; }

tgui::Widget* WidgetEx::operator->() const { return widget_.get(); }

const tgui::Widget::Ptr& WidgetEx::getWidgetPtr() const { return widget_; }

void WidgetEx::getWidget(const cms::string& id) {
  if (id == "root")
    widget_ = FW::G::Window.gui->getContainer();
  else
    widget_ = findWidget(FW::G::Window.gui->getContainer(), id);
}

tgui::Widget::Ptr WidgetEx::findWidget(const tgui::Widget::Ptr& widget,
                                       const cms::string& id) {
  if (widget->getWidgetType() == "GuiContainer") {
    const auto& widgets = widget->cast<tgui::GuiContainer>()->getWidgets();
    for (auto& widget : widgets) {
      try {
        if (widget->getUserData<cms::string>() == id) {
          return widget;
          break;
        } else {
          if (widget->getWidgetType() == "GuiContainer" ||
              widget->getWidgetType() == "ChildWindow") {
            auto found_widget = findWidget(widget, id);
            if (found_widget) return found_widget;
          }
        }
      } catch (...) {}
    }
  } else if (widget->getWidgetType() == "ChildWindow") {
    const auto& widgets = widget->cast<tgui::ChildWindow>()->getWidgets();
    for (auto& widget : widgets) {
      try {
        if (widget->getUserData<cms::string>() == id) {
          return widget;
          break;
        } else {
          if (widget->getWidgetType() == "GuiContainer" ||
              widget->getWidgetType() == "ChildWindow") {
            auto found_widget = findWidget(widget, id);
            if (found_widget) return found_widget;
          }
        }
      } catch (...) {}
    }
  }
  return nullptr;
}