#include "ItemsScreen.h"
#include "Protocol.h"
#include <cstdio>
#include <cstring>

extern Protocol protocol;

// LVGL Objects - Static to this file
static lv_obj_t* main_card = nullptr;
static lv_obj_t* loading_label = nullptr;
static lv_obj_t* arrow_up = nullptr;
static lv_obj_t* arrow_down = nullptr;
static lv_obj_t* page_indicator = nullptr;

// Label objects for item details
static lv_obj_t* label_name = nullptr;
static lv_obj_t* label_item_id = nullptr;
static lv_obj_t* label_current_price = nullptr;
static lv_obj_t* label_next_bid = nullptr;
static lv_obj_t* label_status = nullptr;
static lv_obj_t* label_time_left = nullptr;
static lv_obj_t* label_your_bid = nullptr;

// Current state
static std::vector<ItemData> items;
static String current_auction_id;
static int current_index = 0;
static int total_items = 0;
static bool has_data = false;

// Global items content area (accessible from main code)
lv_obj_t* items_content_area = nullptr;

// Colors
#define COLOR_PRIMARY   lv_color_hex(0x2196F3)
#define COLOR_SUCCESS   lv_color_hex(0x4CAF50)
#define COLOR_DANGER    lv_color_hex(0xFF0000)
#define COLOR_WARNING   lv_color_hex(0xFF9800)
#define COLOR_DARK      lv_color_hex(0x333333)
#define COLOR_WHITE     lv_color_hex(0xFFFFFF)
#define COLOR_BORDER    lv_color_hex(0xDDDDDD)

// Icons (Font Awesome style)
#define ICON_ARROW_UP   "\uF077"
#define ICON_ARROW_DOWN "\uF078"
#define ICON_BOX        "\uF01C"
#define ICON_USER       "\uF07B"
#define ICON_TAG        "\uF021"
#define ICON_SETTINGS   "\uF013"
#define ICON_MONEY      "\uF155"
#define ICON_CLOCK      "\uF017"
#define ICON_CHECK      "\uF00C"

// ================== FORWARD DECLARATIONS ==================
void item_next();
void item_previous();
void refresh_item_display();

// ================== CREATE ITEMS DISPLAY ==================
void create_items_display(lv_obj_t* parent) {
    // Configure parent
    lv_obj_set_style_pad_all(parent, 0, 0);
    lv_obj_set_style_bg_color(parent, COLOR_WHITE, 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
    
    // Loading label
    loading_label = lv_label_create(parent);
    lv_label_set_text(loading_label, "Loading Items...");
    lv_obj_set_style_text_color(loading_label, COLOR_DARK, 0);
    lv_obj_set_style_text_font(loading_label, &lv_font_montserrat_14, 0);
    lv_obj_center(loading_label);
    
    // Top arrow
    arrow_up = lv_label_create(parent);
    lv_label_set_text(arrow_up, ICON_ARROW_UP);
    lv_obj_set_style_text_color(arrow_up, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(arrow_up, &lv_font_montserrat_14, 0);
    lv_obj_align(arrow_up, LV_ALIGN_TOP_MID, 0, 1);
    lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
    
    // Main card
    int card_width = 220;
    int card_height = 280;
    
    main_card = lv_obj_create(parent);
    lv_obj_set_size(main_card, card_width, card_height);
    lv_obj_center(main_card);
    lv_obj_set_style_radius(main_card, 12, 0);
    lv_obj_set_style_border_width(main_card, 2, 0);
    lv_obj_set_style_border_color(main_card, COLOR_BORDER, 0);
    lv_obj_set_style_bg_color(main_card, COLOR_WHITE, 0);
    lv_obj_set_style_shadow_width(main_card, 10, 0);
    lv_obj_set_style_shadow_color(main_card, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_shadow_ofs_x(main_card, 2, 0);
    lv_obj_set_style_shadow_ofs_y(main_card, 2, 0);
    lv_obj_set_style_pad_all(main_card, 15, 0);
    
    // Item Name
    label_name = lv_label_create(main_card);
    lv_obj_set_pos(label_name, 15, 10);
    lv_obj_set_width(label_name, card_width - 30);
    lv_obj_set_style_text_color(label_name, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(label_name, &lv_font_montserrat_14, 0);
    lv_label_set_long_mode(label_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label_name, "Item Name");
    
    // Separator
    lv_obj_t* line1 = lv_obj_create(main_card);
    lv_obj_set_size(line1, card_width - 30, 1);
    lv_obj_set_pos(line1, 0, 35);
    lv_obj_set_style_bg_color(line1, lv_color_hex(0xCCCCCC), 0);
    lv_obj_set_style_border_width(line1, 0, 0);
    lv_obj_set_style_pad_all(line1, 0, 0);
    
    // Item ID
    lv_obj_t* id_icon = lv_label_create(main_card);
    lv_obj_set_pos(id_icon, 15, 45);
    lv_label_set_text(id_icon, ICON_SETTINGS);
    lv_obj_set_style_text_color(id_icon, COLOR_DARK, 0);
    lv_obj_set_style_text_font(id_icon, &lv_font_montserrat_14, 0);
    
    label_item_id = lv_label_create(main_card);
    lv_obj_set_pos(label_item_id, 40, 45);
    lv_obj_set_style_text_color(label_item_id, COLOR_DARK, 0);
    lv_obj_set_style_text_font(label_item_id, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_item_id, "ID: ---");
    
    // Current Price
    lv_obj_t* price_icon = lv_label_create(main_card);
    lv_obj_set_pos(price_icon, 15, 75);
    lv_label_set_text(price_icon, ICON_MONEY);
    lv_obj_set_style_text_color(price_icon, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(price_icon, &lv_font_montserrat_14, 0);
    
    label_current_price = lv_label_create(main_card);
    lv_obj_set_pos(label_current_price, 40, 75);
    lv_obj_set_style_text_color(label_current_price, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(label_current_price, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_current_price, "0.00");
    
    // Next Minimum Bid
    lv_obj_t* next_icon = lv_label_create(main_card);
    lv_obj_set_pos(next_icon, 15, 105);
    lv_label_set_text(next_icon, ICON_TAG);
    lv_obj_set_style_text_color(next_icon, COLOR_WARNING, 0);
    lv_obj_set_style_text_font(next_icon, &lv_font_montserrat_14, 0);
    
    label_next_bid = lv_label_create(main_card);
    lv_obj_set_pos(label_next_bid, 40, 105);
    lv_obj_set_style_text_color(label_next_bid, COLOR_WARNING, 0);
    lv_obj_set_style_text_font(label_next_bid, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_next_bid, "Min: ---");
    
    // Status
    lv_obj_t* status_icon = lv_label_create(main_card);
    lv_obj_set_pos(status_icon, 15, 135);
    lv_label_set_text(status_icon, ICON_CHECK);
    lv_obj_set_style_text_color(status_icon, COLOR_DARK, 0);
    lv_obj_set_style_text_font(status_icon, &lv_font_montserrat_14, 0);
    
    label_status = lv_label_create(main_card);
    lv_obj_set_pos(label_status, 40, 135);
    lv_obj_set_style_text_font(label_status, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_status, "---");
    
    // Time Left
    lv_obj_t* time_icon = lv_label_create(main_card);
    lv_obj_set_pos(time_icon, 15, 165);
    lv_label_set_text(time_icon, ICON_CLOCK);
    lv_obj_set_style_text_color(time_icon, COLOR_DARK, 0);
    lv_obj_set_style_text_font(time_icon, &lv_font_montserrat_14, 0);
    
    label_time_left = lv_label_create(main_card);
    lv_obj_set_pos(label_time_left, 40, 165);
    lv_obj_set_style_text_font(label_time_left, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_time_left, "---");
    
    // Your Bid
    lv_obj_t* your_icon = lv_label_create(main_card);
    lv_obj_set_pos(your_icon, 15, 195);
    lv_label_set_text(your_icon, ICON_USER);
    lv_obj_set_style_text_color(your_icon, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(your_icon, &lv_font_montserrat_14, 0);
    
    label_your_bid = lv_label_create(main_card);
    lv_obj_set_pos(label_your_bid, 40, 195);
    lv_obj_set_style_text_color(label_your_bid, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(label_your_bid, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_your_bid, "No bid placed");
    
    // Bottom arrow
    arrow_down = lv_label_create(parent);
    lv_label_set_text(arrow_down, ICON_ARROW_DOWN);
    lv_obj_set_style_text_color(arrow_down, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(arrow_down, &lv_font_montserrat_14, 0);
    lv_obj_align(arrow_down, LV_ALIGN_BOTTOM_MID, 0, -5);
    
    // Page indicator
    page_indicator = lv_label_create(parent);
    lv_obj_set_style_text_color(page_indicator, COLOR_DARK, 0);
    lv_obj_set_style_text_font(page_indicator, &lv_font_montserrat_14, 0);
    lv_obj_align(page_indicator, LV_ALIGN_BOTTOM_MID, 85, -24);
    lv_label_set_text(page_indicator, "1/1");
    
    // Initially hide main card and show loading
    lv_obj_add_flag(main_card, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
    
    // Setup click events for arrows
    lv_obj_add_event_cb(arrow_up, [](lv_event_t* e) { item_previous(); }, LV_EVENT_CLICKED, nullptr);
    lv_obj_add_event_cb(arrow_down, [](lv_event_t* e) { item_next(); }, LV_EVENT_CLICKED, nullptr);
}

// ================== INIT ITEMS SCREEN ==================
void init_items_screen(lv_obj_t* parent) {
    int screen_w = lv_obj_get_width(parent);
    int screen_h = lv_obj_get_height(parent);
    int top_bar_height = 40;
    
    // Create items content area
    items_content_area = lv_obj_create(parent);
    lv_obj_set_size(items_content_area, screen_w, screen_h - top_bar_height);
    lv_obj_set_pos(items_content_area, 0, top_bar_height);
    lv_obj_set_style_bg_color(items_content_area, COLOR_WHITE, 0);
    lv_obj_set_style_border_width(items_content_area, 0, 0);
    lv_obj_set_style_pad_all(items_content_area, 0, 0);
    
    // Create the items display inside content area
    create_items_display(items_content_area);
    
    Serial.println("Items screen created");
}

// ================== LOAD ITEMS FROM CACHE ==================
void load_items_for_auction(String auctionId) {
    current_auction_id = auctionId;
    items = protocol.getItemsForAuction(auctionId);
    total_items = items.size();
    
    if (total_items > 0) {
        has_data = true;
        current_index = 0;
        refresh_item_display();
        show_items_loading(false);
        Serial.print("Loaded ");
        Serial.print(total_items);
        Serial.println(" items");
    } else {
        has_data = false;
        show_items_loading(false);
        clear_items_screen();
        Serial.println("No items found for this auction");
    }
}

// ================== REFRESH ITEM DISPLAY ==================
void refresh_item_display() {
    if (!has_data || current_index >= total_items) {
        clear_items_screen();
        return;
    }
    
    ItemData item = items[current_index];
    
    // Update labels
    lv_label_set_text(label_name, item.name.c_str());
    
    char id_buf[32];
    snprintf(id_buf, sizeof(id_buf), "%s", item.itemId.c_str());
    lv_label_set_text(label_item_id, id_buf);
    
    char price_buf[32];
    snprintf(price_buf, sizeof(price_buf), "%.2f %s", item.currentPrice, item.currency.c_str());
    lv_label_set_text(label_current_price, price_buf);
    
    char next_bid_buf[32];
    snprintf(next_bid_buf, sizeof(next_bid_buf), "Min: %.2f %s", item.nextMinBid, item.currency.c_str());
    lv_label_set_text(label_next_bid, next_bid_buf);
    
    // Status with color
    lv_label_set_text(label_status, item.status.c_str());
    if (item.status == "OPEN" || item.status == "ACTIVE") {
        lv_obj_set_style_text_color(label_status, COLOR_SUCCESS, 0);
    } else if (item.status == "CLOSED" || item.status == "ENDED") {
        lv_obj_set_style_text_color(label_status, COLOR_DANGER, 0);
    } else {
        lv_obj_set_style_text_color(label_status, COLOR_WARNING, 0);
    }
    
    // Time left
    char time_buf[32];
    if (item.remainingSeconds > 0) {
        int hours = item.remainingSeconds / 3600;
        int minutes = (item.remainingSeconds % 3600) / 60;
        int seconds = item.remainingSeconds % 60;
        if (hours > 0) {
            snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d", hours, minutes, seconds);
        } else {
            snprintf(time_buf, sizeof(time_buf), "%02d:%02d", minutes, seconds);
        }
    } else {
        snprintf(time_buf, sizeof(time_buf), "Ended");
    }
    lv_label_set_text(label_time_left, time_buf);
    
    // Your bid
    if (item.yourBidSubmitted) {
        char your_bid_buf[32];
        snprintf(your_bid_buf, sizeof(your_bid_buf), "Your Bid: %.2f %s", item.yourBidAmount, item.currency.c_str());
        lv_label_set_text(label_your_bid, your_bid_buf);
        lv_obj_set_style_text_color(label_your_bid, COLOR_SUCCESS, 0);
    } else {
        lv_label_set_text(label_your_bid, "No bid placed");
        lv_obj_set_style_text_color(label_your_bid, COLOR_DARK, 0);
    }
    
    // Page indicator
    char page_buf[16];
    snprintf(page_buf, sizeof(page_buf), "%d/%d", current_index + 1, total_items);
    lv_label_set_text(page_indicator, page_buf);
    
    // Show/hide arrows
    if (total_items <= 1) {
        lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
        if (current_index > 0) {
            lv_obj_clear_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
        }
        
        if (current_index < total_items - 1) {
            lv_obj_clear_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    // Show card, hide loading
    lv_obj_clear_flag(main_card, LV_OBJ_FLAG_HIDDEN);
    
    Serial.print("Displaying item: ");
    Serial.print(item.name.c_str());
    Serial.print(" (");
    Serial.print(current_index + 1);
    Serial.print("/");
    Serial.print(total_items);
    Serial.println(")");
}

// ================== ITEM NAVIGATION ==================
void item_next() {
    if (!has_data) return;
    if (current_index < total_items - 1) {
        current_index++;
        refresh_item_display();
        Serial.println("Next item");
    }
}

void item_previous() {
    if (!has_data) return;
    if (current_index > 0) {
        current_index--;
        refresh_item_display();
        Serial.println("Previous item");
    }
}

// ================== UI CONTROL ==================
void show_items_loading(bool show) {
    if (show) {
        lv_obj_clear_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(main_card, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
    }
}

void clear_items_screen() {
    lv_label_set_text(label_name, "No Items");
    lv_label_set_text(label_item_id, "---");
    lv_label_set_text(label_current_price, "0.00");
    lv_label_set_text(label_next_bid, "Min: ---");
    lv_label_set_text(label_status, "---");
    lv_label_set_text(label_time_left, "---");
    lv_label_set_text(label_your_bid, "No bid placed");
    lv_obj_clear_flag(main_card, LV_OBJ_FLAG_HIDDEN);
}

void hide_items_screen() {
    if (items_content_area) {
        lv_obj_add_flag(items_content_area, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Items screen hidden");
    }
}

void show_items_screen() {
    if (items_content_area) {
        lv_obj_clear_flag(items_content_area, LV_OBJ_FLAG_HIDDEN);
        Serial.println("Items screen shown");
    }
}


// ================== GETTERS ==================
const char* get_current_item_id() {
    if (has_data && current_index < total_items) {
        return items[current_index].itemId.c_str();
    }
    return nullptr;
}

ItemData get_current_item() {
    if (has_data && current_index < total_items) {
        return items[current_index];
    }
    return ItemData();
}

int get_current_item_index() {
    return current_index;
}

int get_total_items() {
    return total_items;
}

bool has_items_data() {
    return has_data;
}