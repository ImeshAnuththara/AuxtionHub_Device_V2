// #include "Header_template.h"

// // ================== GLOBAL UI OBJECTS ==================
// lv_obj_t *top_bar;
// lv_obj_t *label_battery;
// lv_obj_t *label_battery_percent;
// lv_obj_t *label_wifi;
// lv_obj_t *label_date;
// lv_obj_t *content_area;
// lv_obj_t *label_mqtt;

// // State variables (optional)
// static bool dark_mode_state;
// static bool wifi_connected_state;
// static bool mqtt_connected_state;

// // ================== TOP BAR CREATION ==================
// void create_topbar(uint16_t screen_width, uint16_t screen_height) {
//     lv_obj_t *scr = lv_scr_act();
//     lv_obj_clean(scr);

//     // Top bar
//     top_bar = lv_obj_create(scr);
//     lv_obj_set_size(top_bar, screen_width, 40);
//     lv_obj_set_pos(top_bar, 0, 0);

//     // Battery icon
//     label_battery = lv_label_create(top_bar);
//     lv_label_set_text(label_battery, "\uF241");
//     lv_obj_align(label_battery, LV_ALIGN_TOP_LEFT, 5, 5);

//     // Battery percentage
//     label_battery_percent = lv_label_create(top_bar);
//     lv_label_set_text(label_battery_percent, "0%");
//     lv_obj_align(label_battery_percent, LV_ALIGN_TOP_LEFT, 30, 5);

//     // Date / Time
//     label_date = lv_label_create(top_bar);
//     lv_label_set_text(label_date, "00:00 01/01");
//     lv_obj_align(label_date, LV_ALIGN_TOP_RIGHT, -5, 5);

//     // WiFi icon
//     label_wifi = lv_label_create(top_bar);
//     lv_label_set_text(label_wifi, "\uF1EB");
//     lv_obj_align(label_wifi, LV_ALIGN_TOP_RIGHT, -92, 5);

//     // MQTT icon
//     label_mqtt = lv_label_create(top_bar);
//     lv_label_set_text(label_mqtt, "\uF6FF");   // Example icon (plug icon from FontAwesome)
//     lv_obj_align(label_mqtt, LV_ALIGN_TOP_RIGHT, -120, 5);

//     // Content area below top bar
//     content_area = lv_obj_create(scr);
//     lv_obj_set_size(content_area, screen_width, screen_height - 40);
//     lv_obj_set_pos(content_area, 0, 40);
//     lv_obj_clear_flag(content_area, LV_OBJ_FLAG_SCROLLABLE);

//     // Apply initial dark mode
//     set_dark_mode(dark_mode_state);
// }

// // ================== HELPER FUNCTIONS ==================
// void set_battery_percent(uint8_t percent) {
//     char buf[6];
//     snprintf(buf, sizeof(buf), "%d%%", percent);
//     lv_label_set_text(label_battery_percent, buf);
// }

// void set_wifi_connected(bool connected) {
//     wifi_connected_state = connected;
//     if (!connected) {
//         lv_obj_add_flag(label_wifi, LV_OBJ_FLAG_HIDDEN); 
//         lv_obj_set_style_bg_opa(label_wifi, LV_OPA_TRANSP, 0);   // transparent background
//         lv_obj_set_style_border_width(label_wifi, 0, 0);         // remove border
//     } else {
//         lv_obj_clear_flag(label_wifi, LV_OBJ_FLAG_HIDDEN); 
//         lv_label_set_text(label_wifi, "\uF1EB");
//     }
// }

// void set_mqtt_connected(bool connected) {
//     mqtt_connected_state = connected;

//     if (!connected) {
//         lv_obj_add_flag(label_mqtt, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_set_style_bg_opa(label_mqtt, LV_OPA_TRANSP, 0);
//         lv_obj_set_style_border_width(label_mqtt, 0, 0);
//     } else {
//         lv_obj_clear_flag(label_mqtt, LV_OBJ_FLAG_HIDDEN);
//         lv_label_set_text(label_mqtt, "\uF074");   // MQTT icon
//     }
// }

// void set_date_time(const char* datetime) {
//     lv_label_set_text(label_date, datetime);
// }

// void set_dark_mode(bool enable) {
//     dark_mode_state = enable;
//     if (enable) {
//         lv_obj_set_style_bg_color(top_bar, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(label_battery, lv_color_hex(0xFFFFFF), 0);
//         lv_obj_set_style_text_color(label_battery_percent, lv_color_hex(0xFFFFFF), 0);
//         lv_obj_set_style_text_color(label_wifi, lv_color_hex(0xFFFFFF), 0);
//         lv_obj_set_style_text_color(label_date, lv_color_hex(0xFFFFFF), 0);
//     } else {
//         lv_obj_set_style_bg_color(top_bar, lv_color_hex(0xFFFFFF), 0);
//         lv_obj_set_style_text_color(label_battery, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(label_battery_percent, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(label_wifi, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(label_date, lv_color_hex(0x000000), 0);
//     }
// }

#include "Header_template.h"
#include <cstdio>

// ================== GLOBAL UI OBJECTS ==================
lv_obj_t *top_bar;
lv_obj_t *label_battery;
lv_obj_t *label_battery_percent;
lv_obj_t *label_wifi;
lv_obj_t *label_date;
lv_obj_t *content_area;
lv_obj_t *label_mqtt;
lv_obj_t *status_message_area;
lv_obj_t *status_icon;
lv_obj_t *status_text;

bool wifi_connected_state;
bool mqtt_connected_state;
static bool status_active = false;
static uint16_t screen_width = 240;
static uint16_t screen_height = 320;

// ================== TOP BAR CREATION ==================
void create_topbar(uint16_t width, uint16_t height) {
    screen_width = width;
    screen_height = height;
    
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);

    // Top bar
    top_bar = lv_obj_create(scr);
    lv_obj_set_size(top_bar, screen_width, 40);
    lv_obj_set_pos(top_bar, 0, 0);
    lv_obj_set_style_bg_color(top_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(top_bar, LV_OBJ_FLAG_HIDDEN); // Ensure visible

    label_battery = lv_label_create(top_bar);
    lv_label_set_text(label_battery, "\uF241");
    lv_obj_align(label_battery, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_text_color(label_battery, lv_color_hex(0x000000), 0);

    label_battery_percent = lv_label_create(top_bar);
    lv_label_set_text(label_battery_percent, "0%");
    lv_obj_align(label_battery_percent, LV_ALIGN_TOP_LEFT, 30, 5);
    lv_obj_set_style_text_color(label_battery_percent, lv_color_hex(0x000000), 0);

    label_date = lv_label_create(top_bar);
    lv_label_set_text(label_date, "00:00 01/01");
    lv_obj_align(label_date, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_set_style_text_color(label_date, lv_color_hex(0x000000), 0);

    label_wifi = lv_label_create(top_bar);
    lv_label_set_text(label_wifi, "\uF1EB");
    lv_obj_align(label_wifi, LV_ALIGN_TOP_RIGHT, -92, 5);
    lv_obj_set_style_text_color(label_wifi, lv_color_hex(0x000000), 0);

    label_mqtt = lv_label_create(top_bar);
    lv_label_set_text(label_mqtt, "\uF6FF");
    lv_obj_align(label_mqtt, LV_ALIGN_TOP_RIGHT, -120, 5);
    lv_obj_set_style_text_color(label_mqtt, lv_color_hex(0x000000), 0);

    content_area = lv_obj_create(scr);
    lv_obj_set_size(content_area, screen_width, screen_height - 40);
    lv_obj_set_pos(content_area, 0, 40);
    lv_obj_set_style_bg_color(content_area, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(content_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(content_area, LV_OBJ_FLAG_HIDDEN);
    
    // Status screen - full screen, hidden by default
    status_message_area = lv_obj_create(scr);
    lv_obj_set_size(status_message_area, screen_width, screen_height);
    lv_obj_set_pos(status_message_area, 0, 0);
    lv_obj_set_style_bg_color(status_message_area, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(status_message_area, 0, 0);
    lv_obj_add_flag(status_message_area, LV_OBJ_FLAG_HIDDEN);
    
    // Center container
    lv_obj_t *center = lv_obj_create(status_message_area);
    lv_obj_set_size(center, screen_width, screen_height);
    lv_obj_set_pos(center, 0, 0);
    lv_obj_set_style_bg_opa(center, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(center, 0, 0);
    lv_obj_set_flex_flow(center, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(center, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    status_icon = lv_label_create(center);
    lv_label_set_text(status_icon, "\uF110");
    lv_obj_set_style_text_font(status_icon, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(status_icon, lv_color_hex(0x000000), 0);
    
    status_text = lv_label_create(center);
    lv_label_set_text(status_text, "");
    lv_obj_set_style_text_font(status_text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(status_text, lv_color_hex(0x000000), 0);
}

// ================== HELPER FUNCTIONS ==================
void set_battery_percent(uint8_t percent) {
    if (!label_battery_percent) return;
    char buf[6];
    sprintf(buf, "%d%%", percent);
    lv_label_set_text(label_battery_percent, buf);
}

void set_wifi_connected(bool connected) {
    wifi_connected_state = connected;
    if (!label_wifi) return;
    
    if (!connected) {
        lv_obj_add_flag(label_wifi, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(label_wifi, LV_OBJ_FLAG_HIDDEN);
    }
}

void set_mqtt_connected(bool connected) {
    mqtt_connected_state = connected;
    if (!label_mqtt) return;
    
    if (!connected) {
        lv_obj_add_flag(label_mqtt, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(label_mqtt, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(label_mqtt, "\uF074");
    }
}

void set_date_time(const char* datetime) {
    if (label_date) {
        lv_label_set_text(label_date, datetime);
    }
}

void set_dark_mode(bool enable) {
    // Not used - keep light mode
}

// ================== STATUS FUNCTIONS ==================
void show_status_message(const char* message, const char* icon) {
    if (!status_message_area) return;
    
    // Hide top bar and content area
    if (top_bar) lv_obj_add_flag(top_bar, LV_OBJ_FLAG_HIDDEN);
    if (content_area) lv_obj_add_flag(content_area, LV_OBJ_FLAG_HIDDEN);
    
    // Set text and icon
    if (status_icon) lv_label_set_text(status_icon, icon);
    if (status_text) lv_label_set_text(status_text, message);
    
    // Show status area
    lv_obj_clear_flag(status_message_area, LV_OBJ_FLAG_HIDDEN);
    status_active = true;
    
    // Force LVGL to refresh
    lv_task_handler();
}

void hide_status_message() {
    if (!status_message_area) return;
    
    // Hide status area
    lv_obj_add_flag(status_message_area, LV_OBJ_FLAG_HIDDEN);
    
    // Show top bar and content area
    if (top_bar) lv_obj_clear_flag(top_bar, LV_OBJ_FLAG_HIDDEN);
    if (content_area) lv_obj_clear_flag(content_area, LV_OBJ_FLAG_HIDDEN);
    
    status_active = false;
    
    // Force LVGL to refresh
    lv_task_handler();
}

void update_status_message_text(const char* message) {
    if (status_active && status_text) {
        lv_label_set_text(status_text, message);
        lv_task_handler();
    }
}

void update_status_icon(const char* icon) {
    if (status_active && status_icon) {
        lv_label_set_text(status_icon, icon);
        lv_task_handler();
    }
}

// ================== NEW FUNCTIONS FOR BETTER SCREEN MANAGEMENT ==================
void clear_all_and_show_topbar() {
    // First hide any status message
    hide_status_message();
    
    // Clear content area
    if (content_area) {
        lv_obj_clean(content_area);
    }
}

void refresh_topbar() {
    // Force refresh of top bar items
    if (top_bar) {
        lv_obj_invalidate(top_bar);
    }
    if (label_battery_percent) {
        lv_obj_invalidate(label_battery_percent);
    }
    if (label_date) {
        lv_obj_invalidate(label_date);
    }
    if (label_wifi) {
        lv_obj_invalidate(label_wifi);
    }
    if (label_mqtt) {
        lv_obj_invalidate(label_mqtt);
    }
    lv_task_handler();
}

bool is_status_message_active() {
    return status_active;
}

// ================== UTILITY FUNCTIONS ==================
void show_temporary_message(const char* message, const char* icon, uint32_t duration_ms) {
    show_status_message(message, icon);
    
    // Auto-hide after duration
    lv_timer_t *timer = lv_timer_create([](lv_timer_t *t) {
        hide_status_message();
        lv_timer_del(t);
    }, duration_ms, NULL);
    lv_timer_set_repeat_count(timer, 1);
}