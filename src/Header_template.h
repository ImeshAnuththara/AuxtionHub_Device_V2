#ifndef HEADER_TEMPLATE_H
#define HEADER_TEMPLATE_H

#include <lvgl.h>

// ================== GLOBAL UI OBJECTS ==================
extern lv_obj_t *top_bar;
extern lv_obj_t *label_battery;
extern lv_obj_t *label_battery_percent;
extern lv_obj_t *label_wifi;
extern lv_obj_t *label_date;
extern lv_obj_t *content_area;
extern lv_obj_t *label_mqtt;
extern lv_obj_t *status_message_area;
extern lv_obj_t *status_icon;
extern lv_obj_t *status_text;

// State variables
extern bool wifi_connected_state;
extern bool mqtt_connected_state;

// ================== FUNCTION DECLARATIONS ==================
void create_topbar(uint16_t screen_width, uint16_t screen_height);
void set_battery_percent(uint8_t percent);
void set_wifi_connected(bool connected);
void set_mqtt_connected(bool connected);
void set_date_time(const char* datetime);
void set_dark_mode(bool enable);

// Status message functions
void show_status_message(const char* message, const char* icon = "\uF110");
void hide_status_message();
void update_status_message_text(const char* message);
void update_status_icon(const char* icon);

#endif