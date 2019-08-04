#include <cmsext/cmsext.h>

CMS_BEGIN(ui_mod, "")
cms::string label_click;

int click_count = -1;

void update_label_click() {
  click_count += 1;
  label_set_text(label_click, "Clicked: " + cms::str(click_count));
}

void on_press_button_exit() { cmsext::app::close(); }

void createUI() {
  {
    cms::string root = "root";
    auto picture =
        picture_create(root, get_assets_path() + "/image/RedBackground.jpg");

    auto tabs = tabs_create(root);
    widget_set_position(tabs, 70, 40);
    tabs_set_tab_height(tabs, 30);
    tabs_add_tab(tabs, "Tab - 1");
    tabs_add_tab(tabs, "Tab - 2");
    tabs_add_tab(tabs, "Tab - 3");

    auto menu_bar = menu_bar_create(root);
    widget_set_size(menu_bar, get_window_width(), 22);
    menu_bar_add_menu(menu_bar, "File");
    menu_bar_add_menu_item(menu_bar, "Load");
    menu_bar_add_menu_item(menu_bar, "Save");
    menu_bar_add_menu_item(menu_bar, "Exit");
    menu_bar_add_menu(menu_bar, "Edit");
    menu_bar_add_menu_item(menu_bar, "Copy");
    menu_bar_add_menu_item(menu_bar, "Paste");
    menu_bar_add_menu(menu_bar, "Help");
    menu_bar_add_menu_item(menu_bar, "About");

    auto label = label_create(root);
    widget_set_position(label, 10, 90);
    label_set_text(label, "This is a label.\nAnd these are radio buttons:");
    label_set_text_size(label, 18);

    auto radio_button = radio_button_create(root);
    widget_set_position(radio_button, 20, 140);
    widget_set_size(radio_button, 25, 25);
    radio_button_set_text(radio_button, "Yep!");

    radio_button = radio_button_create(root);
    widget_set_position(radio_button, 20, 170);
    widget_set_size(radio_button, 25, 25);
    radio_button_set_text(radio_button, "Nope!");

    radio_button = radio_button_create(root);
    widget_set_position(radio_button, 20, 200);
    widget_set_size(radio_button, 25, 25);
    radio_button_set_text(radio_button, "Don't know!");

    label = label_create(root);
    widget_set_position(label, 10, 240);
    label_set_text(label, "We've got some edit boxes:");
    label_set_text_size(label, 18);

    auto editbox = edit_box_create(root);
    widget_set_size(editbox, 200, 25);
    widget_set_position(editbox, 10, 270);
    edit_box_set_text_size(editbox, 18);
    edit_box_set_default_text(editbox, "Click to edit text...");

    label = label_create(root);
    widget_set_position(label, 10, 310);
    label_set_text(label, "And some list boxes too...");
    label_set_text_size(label, 18);

    auto list_box = list_box_create(root);
    widget_set_size(list_box, 250, 120);
    widget_set_position(list_box, 10, 340);
    list_box_set_item_height(list_box, 24);
    list_box_add_item(list_box, "Item 1");
    list_box_add_item(list_box, "Item 2");
    list_box_add_item(list_box, "Item 3");

    label = label_create(root);
    widget_set_position(label, 10, 470);
    label_set_text(label, "It's the progress bar below");
    label_set_text_size(label, 18);

    auto progress_bar = progress_bar_create(root);
    widget_set_position(progress_bar, 10, 500);
    widget_set_size(progress_bar, 200, 20);
    progress_bar_set_value(progress_bar, 50);

    label = label_create(root);
    widget_set_position(label, 220, 500);
    label_set_text(label,
                   std::to_string(progress_bar_get_value(progress_bar)) + "%");
    label_set_text_size(label, 18);

    label = label_create(root);
    widget_set_position(label, 10, 530);
    label_set_text(label, "That's the slider");
    label_set_text_size(label, 18);

    auto slider = slider_create(root);
    widget_set_position(slider, 10, 560);
    widget_set_size(slider, 200, 18);
    slider_set_value(slider, 4);

    auto scrollbar = scrollbar_create(root);
    widget_set_position(scrollbar, 380, 40);
    widget_set_size(scrollbar, 18, 540);
    scrollbar_create_set_maximum(scrollbar, 100);
    scrollbar_create_set_viewport_size(scrollbar, 70);

    auto comb_box = combo_box_create(root);
    widget_set_position(comb_box, 420, 40);
    widget_set_size(comb_box, 120, 21);
    combo_box_add_item(comb_box, "Item 1");
    combo_box_add_item(comb_box, "Item 2");
    combo_box_add_item(comb_box, "Item 3");
    combo_box_set_selected_item(comb_box, "Item 2");

    auto child_window = child_window_create(root);
    widget_set_position(child_window, 420, 80);
    widget_set_size(child_window, 250, 120);
    child_window_set_title(child_window, "Child window");

    label_click = label_create(child_window);
    widget_set_position(label_click, 30, 30);
    label_set_text_size(label_click, 15);
    update_label_click();

    auto button = button_create(child_window);
    widget_set_position(button, 75, 70);
    widget_set_size(button, 100, 30);
    widget_connect(button, "pressed", update_label_click);
    button_set_text(button, "OK");

    auto check_box = check_box_create(root);
    widget_set_position(check_box, 420, 240);
    widget_set_size(check_box, 25, 25);
    check_box_set_text(check_box, "Ok, I got it " + cms::string(__TIME__));

    check_box = check_box_create(root);
    widget_set_position(check_box, 570, 240);
    widget_set_size(check_box, 25, 25);
    check_box_set_text(check_box, "No, I didn't");

    label = label_create(root);
    widget_set_position(label, 420, 280);
    label_set_text(label, "Chatbox");
    label_set_text_size(label, 18);

    auto chat_box = chat_box_create(root);
    widget_set_position(chat_box, 420, 310);
    widget_set_size(chat_box, 300, 100);
    chat_box_set_text_size(chat_box, 18);
    chat_box_set_lines_start_from_top(chat_box);
    chat_box_add_line(chat_box, "texus: Hey, this is TGUI!", 0, 128, 0, 255);
    chat_box_add_line(chat_box, "Me: Looks awesome! ;)", 255, 255, 0, 255);
    chat_box_add_line(chat_box, "texus: Thanks! :)", 0, 128, 0, 255);
    chat_box_add_line(chat_box, "Me: The widgets rock ^^", 255, 255, 0, 255);

    auto canvas = canvas_create(root);
    widget_set_position(canvas, 420, 430);
    widget_set_size(canvas, 200, 140);
    canvas_clear(canvas);
    canvas_draw_sprite(canvas, get_assets_path() + "/image/ThinkLinux.jpg", 200,
                       140);
    // TODO
    // Dont work. Fix it.
    // canvas_draw_text(canvas, "SFML Canvas", 25, 100, 24, 200, 200, 200);
    canvas_display(canvas);

    auto button_exit = button_create(root);
    widget_set_position(button_exit, get_window_width() - 115,
                        get_window_height() - 50);
    widget_set_size(button_exit, 100, 40);
    widget_connect(button_exit, "pressed", on_press_button_exit);
    button_set_text(button_exit, "Exit");
  }
}

void on_load() {}
void on_init() { createUI(); }
void on_terminate() {}
void on_reload() {}
CMS_END()
