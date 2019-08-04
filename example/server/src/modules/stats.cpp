#include <cmsext/db.h>
#include <cmsext/gui.h>
#include <cmsext/log.h>
#include <cmsext/net.h>
#include <cmsext/thread.h>
#include <cmsext/widget.h>

using namespace cmsext::widget;
using namespace cmsext::net;
using namespace cmsext::gui;
using namespace cmsext::log;
using namespace cmsext::thread;
using namespace cmsext::db;

CMS_BEGIN(stats, "")
cms::string list_box_clients;
cms::string label_max_clients_online;
int max_clients_online = -1;
int current_clients_online = 0;
int sql_id = -1;

void update_label() {
  label_set_text(label_max_clients_online,
                 "Max clients online: " + cms::str(max_clients_online));
}

void update_clients_online() {
  list_box_clear_items(list_box_clients);
  current_clients_online = 0;

  cms::immutable_list<int> ids;
  tcp_listener_get_connections(0, ids);
  int i = 0;
  while (i < cms::len(ids)) {
    if (is_client_connected(ids[i]) == true) {
      list_box_add_item(list_box_clients, "ID:" + cms::str(ids[i]));
      current_clients_online += 1;
    }

    i += 1;
  }
}

void set_up_max_clients_online() {
  sql_id =
      cmsext::db::connect("mysql", "127.0.0.1", "root", "", "test", "3306", "");

  if (sql_id == -1) {
    error("Unable to connect to database.");
    return;
  }

  if (!execute_sync(sql_id, "SELECT `max_clients` FROM `stats`")) {
    error("Unable to execute query.");
    return;
  }

  max_clients_online = get_number(sql_id, "max_clients");
  update_label();
}

void update_max_clients_online() {
  if (current_clients_online > max_clients_online) {
    max_clients_online = current_clients_online;
    update_label();

    if (!execute_sync(sql_id, "UPDATE `stats` SET `max_clients`=" +
                                  cms::str(max_clients_online))) {
      error("Unable to execute query.");
      return;
    }
  }
}

void create_ui() {
  cms::string root = "root";

  label_max_clients_online = label_create(root);
  widget_set_position(label_max_clients_online, 300, 5);
  widget_set_size(label_max_clients_online, 200, 40);
  label_set_text(label_max_clients_online, "");
  label_set_text_size(label_max_clients_online, 18);

  auto label_clients = label_create(root);
  widget_set_position(label_clients, get_window_width() - 95, 5);
  widget_set_size(label_clients, 95, 40);
  label_set_text(label_clients, "Online:");
  label_set_text_size(label_clients, 18);

  list_box_clients = list_box_create(root);
  widget_set_position(list_box_clients, get_window_width() - 95, 30);
  widget_set_size(list_box_clients, 90, get_window_height() - 40);
  list_box_set_item_height(list_box_clients, 18);

  add_sync_event(update_clients_online, 1, 0, 100);

  set_up_max_clients_online();
  add_sync_event(update_max_clients_online, 1, 0, 100);
}
// Signals
void on_load() {}
void on_init() { create_ui(); }
void on_terminate() {}
void on_reload() {}

CMS_END()
