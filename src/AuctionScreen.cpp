// #include "AuctionScreen.h"
// #include "Protocol.h"
// #include <cstdio>
// #include <cstring>

// // External Protocol instance (declared in main)
// extern Protocol protocol;

// // LVGL Objects
// static lv_obj_t* content_area = nullptr;
// static lv_obj_t* card = nullptr;
// static lv_obj_t* label_name = nullptr;
// static lv_obj_t* label_id = nullptr;
// static lv_obj_t* label_mode = nullptr;
// static lv_obj_t* label_status = nullptr;
// static lv_obj_t* label_start = nullptr;
// static lv_obj_t* label_end = nullptr;
// static lv_obj_t* label_items = nullptr;
// static lv_obj_t* label_users = nullptr;
// static lv_obj_t* arrow_up = nullptr;
// static lv_obj_t* arrow_down = nullptr;
// static lv_obj_t* page_label = nullptr;
// static lv_obj_t* loading_label = nullptr;
// static lv_obj_t* error_label = nullptr;

// // Current state
// static std::vector<String> auction_ids;
// static int current_index = 0;
// static int total_auctions = 0;
// static bool is_loading = false;
// static bool has_data = false;

// // Colors
// #define COLOR_PRIMARY   lv_color_hex(0x2196F3)
// #define COLOR_SUCCESS   lv_color_hex(0x4CAF50)
// #define COLOR_DANGER    lv_color_hex(0xFF0000)
// #define COLOR_WARNING   lv_color_hex(0xFF9800)
// #define COLOR_DARK      lv_color_hex(0x333333)
// #define COLOR_WHITE     lv_color_hex(0xFFFFFF)
// #define COLOR_BORDER    lv_color_hex(0xDDDDDD)

// // Icons
// #define ICON_UP         "\uF077"
// #define ICON_DOWN       "\uF078"
// #define ICON_CALENDAR   "\uF00B"
// #define ICON_BOX        "\uF01C"
// #define ICON_USER       "\uF07B"
// #define ICON_TAG        "\uF021"
// #define ICON_SETTINGS   "\uF013"
// #define ICON_MODE_OPEN  "\uF06E"
// #define ICON_MODE_CLOSED "\uF070"

// // ================== FORWARD DECLARATIONS ==================
// void auction_next();
// void auction_previous();
// void refresh_display();

// // ================== CREATE AUCTION SCREEN ==================
// void init_auction_screen(lv_obj_t* parent) {
//     // Get screen dimensions
//     int screen_w = lv_obj_get_width(parent);
//     int screen_h = lv_obj_get_height(parent);
//     int top_bar_height = 40;
    
//     // Content area below top bar
//     content_area = lv_obj_create(parent);
//     lv_obj_set_size(content_area, screen_w, screen_h - top_bar_height);
//     lv_obj_set_pos(content_area, 0, top_bar_height);
//     lv_obj_set_style_bg_color(content_area, lv_color_hex(0xF5F5F5), 0);
//     lv_obj_set_style_border_width(content_area, 0, 0);
//     lv_obj_set_style_pad_all(content_area, 0, 0);
    
//     // Loading label
//     loading_label = lv_label_create(content_area);
//     lv_label_set_text(loading_label, "Loading Auctions...");
//     lv_obj_center(loading_label);
    
//     // Error label
//     error_label = lv_label_create(content_area);
//     lv_label_set_text(error_label, "Failed to load auctions.\nPress OK to retry");
//     lv_obj_center(error_label);
//     lv_obj_set_style_text_align(error_label, LV_TEXT_ALIGN_CENTER, 0);
//     lv_obj_add_flag(error_label, LV_OBJ_FLAG_HIDDEN);
    
//     // Navigation arrows
//     arrow_up = lv_label_create(content_area);
//     lv_label_set_text(arrow_up, ICON_UP);
//     lv_obj_set_style_text_font(arrow_up, &lv_font_montserrat_14, 0);
//     lv_obj_set_style_text_color(arrow_up, COLOR_DANGER, 0);
//     lv_obj_align(arrow_up, LV_ALIGN_TOP_MID, 0, 5);
//     lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
    
//     arrow_down = lv_label_create(content_area);
//     lv_label_set_text(arrow_down, ICON_DOWN);
//     lv_obj_set_style_text_font(arrow_down, &lv_font_montserrat_14, 0);
//     lv_obj_set_style_text_color(arrow_down, COLOR_DANGER, 0);
//     lv_obj_align(arrow_down, LV_ALIGN_BOTTOM_MID, 0, -5);
//     lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
    
//     // Page indicator
//     page_label = lv_label_create(content_area);
//     lv_obj_set_style_text_font(page_label, &lv_font_montserrat_14, 0);
//     lv_obj_align(page_label, LV_ALIGN_BOTTOM_MID, 85, -24);
//     lv_label_set_text(page_label, "1/1");
//     lv_obj_add_flag(page_label, LV_OBJ_FLAG_HIDDEN);
    
//     // Main card
//     card = lv_obj_create(content_area);
//     lv_obj_set_size(card, screen_w - 20, (screen_h - top_bar_height) - 70);
//     lv_obj_center(card);
//     lv_obj_set_style_radius(card, 10, 0);
//     lv_obj_set_style_border_width(card, 1, 0);
//     lv_obj_set_style_border_color(card, COLOR_BORDER, 0);
//     lv_obj_set_style_bg_color(card, COLOR_WHITE, 0);
//     lv_obj_set_style_pad_all(card, 10, 0);
    
//     // Card content
//     int y = 5;
    
//     // Name
//     label_name = lv_label_create(card);
//     lv_obj_set_pos(label_name, 10, y);
//     lv_obj_set_width(label_name, screen_w - 40);
//     lv_obj_set_style_text_color(label_name, COLOR_DANGER, 0);
//     lv_obj_set_style_text_font(label_name, &lv_font_montserrat_14, 0);
    
//     y += 30;
    
//     // Separator
//     lv_obj_t* line = lv_obj_create(card);
//     lv_obj_set_size(line, screen_w - 40, 1);
//     lv_obj_set_pos(line, 0, y);
//     lv_obj_set_style_bg_color(line, COLOR_BORDER, 0);
//     lv_obj_set_style_border_width(line, 0, 0);
    
//     y += 8;
    
//     // ID
//     lv_obj_t* id_icon = lv_label_create(card);
//     lv_obj_set_pos(id_icon, 10, y);
//     lv_label_set_text(id_icon, ICON_SETTINGS);
//     lv_obj_set_style_text_font(id_icon, &lv_font_montserrat_14, 0);
    
//     label_id = lv_label_create(card);
//     lv_obj_set_pos(label_id, 30, y);
//     lv_obj_set_style_text_font(label_id, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // Start Date
//     lv_obj_t* start_icon = lv_label_create(card);
//     lv_obj_set_pos(start_icon, 10, y);
//     lv_label_set_text(start_icon, ICON_CALENDAR);
//     lv_obj_set_style_text_font(start_icon, &lv_font_montserrat_14, 0);
    
//     label_start = lv_label_create(card);
//     lv_obj_set_pos(label_start, 30, y);
//     lv_obj_set_style_text_font(label_start, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // End Date
//     lv_obj_t* end_icon = lv_label_create(card);
//     lv_obj_set_pos(end_icon, 10, y);
//     lv_label_set_text(end_icon, ICON_CALENDAR);
//     lv_obj_set_style_text_color(end_icon, COLOR_DANGER, 0);
//     lv_obj_set_style_text_font(end_icon, &lv_font_montserrat_14, 0);
    
//     label_end = lv_label_create(card);
//     lv_obj_set_pos(label_end, 30, y);
//     lv_obj_set_style_text_color(label_end, COLOR_DANGER, 0);
//     lv_obj_set_style_text_font(label_end, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // Mode
//     lv_obj_t* mode_icon = lv_label_create(card);
//     lv_obj_set_pos(mode_icon, 10, y);
//     lv_label_set_text(mode_icon, ICON_MODE_OPEN);
//     lv_obj_set_style_text_font(mode_icon, &lv_font_montserrat_14, 0);
    
//     label_mode = lv_label_create(card);
//     lv_obj_set_pos(label_mode, 30, y);
//     lv_obj_set_style_text_font(label_mode, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // Status
//     lv_obj_t* status_icon = lv_label_create(card);
//     lv_obj_set_pos(status_icon, 10, y);
//     lv_label_set_text(status_icon, ICON_TAG);
//     lv_obj_set_style_text_font(status_icon, &lv_font_montserrat_14, 0);
    
//     label_status = lv_label_create(card);
//     lv_obj_set_pos(label_status, 30, y);
//     lv_obj_set_style_text_font(label_status, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // Items
//     lv_obj_t* items_icon = lv_label_create(card);
//     lv_obj_set_pos(items_icon, 10, y);
//     lv_label_set_text(items_icon, ICON_BOX);
//     lv_obj_set_style_text_color(items_icon, COLOR_PRIMARY, 0);
//     lv_obj_set_style_text_font(items_icon, &lv_font_montserrat_14, 0);
    
//     label_items = lv_label_create(card);
//     lv_obj_set_pos(label_items, 30, y);
//     lv_obj_set_style_text_color(label_items, COLOR_PRIMARY, 0);
//     lv_obj_set_style_text_font(label_items, &lv_font_montserrat_14, 0);
    
//     lv_obj_t* items_text = lv_label_create(card);
//     lv_obj_set_pos(items_text, 48, y);
//     lv_label_set_text(items_text, "Items");
//     lv_obj_set_style_text_font(items_text, &lv_font_montserrat_14, 0);
    
//     y += 24;
    
//     // Users
//     lv_obj_t* users_icon = lv_label_create(card);
//     lv_obj_set_pos(users_icon, 10, y);
//     lv_label_set_text(users_icon, ICON_USER);
//     lv_obj_set_style_text_color(users_icon, COLOR_SUCCESS, 0);
//     lv_obj_set_style_text_font(users_icon, &lv_font_montserrat_14, 0);
    
//     label_users = lv_label_create(card);
//     lv_obj_set_pos(label_users, 30, y);
//     lv_obj_set_style_text_color(label_users, COLOR_SUCCESS, 0);
//     lv_obj_set_style_text_font(label_users, &lv_font_montserrat_14, 0);
    
//     lv_obj_t* users_text = lv_label_create(card);
//     lv_obj_set_pos(users_text, 48, y);
//     lv_label_set_text(users_text, "Registered");
//     lv_obj_set_style_text_font(users_text, &lv_font_montserrat_14, 0);
    
//     // Initially hide card
//     lv_obj_add_flag(card, LV_OBJ_FLAG_HIDDEN);
    
//     // Setup click events
//     lv_obj_add_event_cb(arrow_up, [](lv_event_t* e) { auction_previous(); }, LV_EVENT_CLICKED, nullptr);
//     lv_obj_add_event_cb(arrow_down, [](lv_event_t* e) { auction_next(); }, LV_EVENT_CLICKED, nullptr);
// }

// // ================== LOAD AUCTIONS FROM CACHE ==================
// void load_auctions_from_cache() {
//     // Get auction IDs from protocol cache
//     auction_ids = protocol.getAuctionIds();
//     total_auctions = auction_ids.size();
    
//     if (total_auctions > 0) {
//         has_data = true;
//         current_index = 0;
        
//         // Set active auction in protocol
//         if (current_index < total_auctions) {
//             protocol.setActiveAuction(auction_ids[current_index]);
//         }
        
//         refresh_display();
//         show_auction_loading(false);
//         lv_obj_add_flag(error_label, LV_OBJ_FLAG_HIDDEN);
//     } else {
//         // No auctions in cache
//         has_data = false;
//         show_auction_loading(false);
//         clear_auction_screen();
//     }
// }

// // ================== REFRESH DISPLAY ==================
// void refresh_display() {
//     if (!has_data || current_index >= total_auctions) {
//         clear_auction_screen();
//         return;
//     }
    
//     // Get auction data from cache
//     String auctionId = auction_ids[current_index];
//     AuctionData auction = protocol.getAuctionData(auctionId);
    
//     if (auction.auctionId.isEmpty()) {
//         // Auction not found in cache
//         clear_auction_screen();
//         return;
//     }
    
//     // Update labels
//     lv_label_set_text(label_name, auction.name.c_str());
    
//     char id_buf[32];
//     snprintf(id_buf, sizeof(id_buf), "ID: %s", auction.auctionId.c_str());
//     lv_label_set_text(label_id, id_buf);
    
//     // Format dates (extract time part if needed)
//     char start_buf[32];
//     char end_buf[32];
    
//     // Assuming dates are in format "YYYY-MM-DD HH:MM:SS" or "YYYY-MM-DDTHH:MM:SS"
//     String startDate = auction.startDateTime;
//     String endDate = auction.endDateTime;
    
//     // Extract only date part if it's too long
//     if (startDate.length() > 16) {
//         startDate = startDate.substring(0, 10); // Get YYYY-MM-DD
//     }
//     if (endDate.length() > 16) {
//         endDate = endDate.substring(0, 10);
//     }
    
//     snprintf(start_buf, sizeof(start_buf), "Start: %s", startDate.c_str());
//     snprintf(end_buf, sizeof(end_buf), "End: %s", endDate.c_str());
//     lv_label_set_text(label_start, start_buf);
//     lv_label_set_text(label_end, end_buf);
    
//     // Mode with icon
//     String modeText = auction.mode;
//     if (modeText == "ENGLISH") {
//         modeText = "English Auction";
//     } else if (modeText == "CLOSED") {
//         modeText = "Closed Bid";
//     }
//     lv_label_set_text(label_mode, modeText.c_str());
    
//     // Status with color
//     String statusText = auction.status;
//     lv_label_set_text(label_status, statusText.c_str());
    
//     // Status color coding
//     if (statusText == "ACTIVE" || statusText == "ONGOING") {
//         lv_obj_set_style_text_color(label_status, COLOR_SUCCESS, 0);
//     } else if (statusText == "UPCOMING" || statusText == "SCHEDULED") {
//         lv_obj_set_style_text_color(label_status, COLOR_WARNING, 0);
//     } else if (statusText == "ENDED" || statusText == "COMPLETED") {
//         lv_obj_set_style_text_color(label_status, COLOR_DANGER, 0);
//     } else {
//         lv_obj_set_style_text_color(label_status, COLOR_DARK, 0);
//     }
    
//     // Items count
//     char items_buf[16];
//     snprintf(items_buf, sizeof(items_buf), "%d", auction.itemsCount);
//     lv_label_set_text(label_items, items_buf);
    
//     // Users count (if available, otherwise show placeholder)
//     char users_buf[16];
//     // Note: registered_count might not be in your AuctionData struct
//     // You can add it or show items count
//     snprintf(users_buf, sizeof(users_buf), "%d", auction.itemsCount);
//     lv_label_set_text(label_users, users_buf);
    
//     // Page indicator
//     char page_buf[16];
//     snprintf(page_buf, sizeof(page_buf), "%d/%d", current_index + 1, total_auctions);
//     lv_label_set_text(page_label, page_buf);
    
//     // Show/hide arrows
//     if (total_auctions <= 1) {
//         lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(page_label, LV_OBJ_FLAG_HIDDEN);
//     } else {
//         lv_obj_clear_flag(page_label, LV_OBJ_FLAG_HIDDEN);
//         if (current_index > 0) {
//             lv_obj_clear_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//         } else {
//             lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//         }
        
//         if (current_index < total_auctions - 1) {
//             lv_obj_clear_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//         } else {
//             lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//         }
//     }
    
//     // Show card
//     lv_obj_clear_flag(card, LV_OBJ_FLAG_HIDDEN);
    
//     Serial.print("Displaying auction: ");
//     Serial.print(auction.name.c_str());
//     Serial.print(" (");
//     Serial.print(current_index + 1);
//     Serial.print("/");
//     Serial.print(total_auctions);
//     Serial.println(")");
// }

// // ================== AUCTION NAVIGATION ==================
// void auction_next() {
//     if (!has_data) return;
//     if (current_index < total_auctions - 1) {
//         current_index++;
//         protocol.setActiveAuction(auction_ids[current_index]);
//         refresh_display();
//     }
// }

// void auction_previous() {
//     if (!has_data) return;   
//     if (current_index > 0) {
//         current_index--;
//         protocol.setActiveAuction(auction_ids[current_index]);
//         refresh_display();
//     }
// }

// void auction_set_index(int index) {
//     if (!has_data || index < 0 || index >= total_auctions) return;
    
//     current_index = index;
//     protocol.setActiveAuction(auction_ids[current_index]);
//     refresh_display();
// }

// void auction_refresh() {
//     // Reload from cache and refresh display
//     load_auctions_from_cache();
// }

// // ================== SHOW/HIDE LOADING ==================
// void show_auction_loading(bool show) {
//     is_loading = show;
    
//     if (show) {
//         lv_obj_clear_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(card, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(page_label, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(error_label, LV_OBJ_FLAG_HIDDEN);
//     } else {
//         lv_obj_add_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
//     }
// }

// void show_auction_error(const char* message) {
//     lv_label_set_text(error_label, message);
//     lv_obj_clear_flag(error_label, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_add_flag(card, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_add_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
//     is_loading = false;
// }

// // ================== CLEAR SCREEN ==================
// void clear_auction_screen() {
//     lv_label_set_text(label_name, "No Auctions");
//     lv_label_set_text(label_id, "ID: ---");
//     lv_label_set_text(label_start, "Start: ---");
//     lv_label_set_text(label_end, "End: ---");
//     lv_label_set_text(label_mode, "---");
//     lv_label_set_text(label_status, "---");
//     lv_label_set_text(label_items, "0");
//     lv_label_set_text(label_users, "0");
//     lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_add_flag(page_label, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_clear_flag(card, LV_OBJ_FLAG_HIDDEN);
// }

// // ================== GET CURRENT AUCTION INFO ==================
// const char* get_current_auction_id() {
//     if (has_data && current_index < total_auctions) {
//         return auction_ids[current_index].c_str();
//     }
//     return nullptr;
// }

// AuctionData get_current_auction() {
//     if (has_data && current_index < total_auctions) {
//         return protocol.getAuctionData(auction_ids[current_index]);
//     }
//     return AuctionData();
// }

// int get_current_auction_index() {
//     return current_index;
// }

// int get_total_auctions() {
//     return total_auctions;
// }

// bool has_auction_data() {
//     return has_data;
// }

#include "AuctionScreen.h"
#include "Protocol.h"
#include <cstdio>
#include <cstring>

// External Protocol instance
extern Protocol protocol;

// LVGL Objects
static lv_obj_t* content_area = nullptr;
static lv_obj_t* main_card = nullptr;
//static lv_obj_t* loading_label = nullptr;
static lv_obj_t* arrow_up = nullptr;
static lv_obj_t* arrow_down = nullptr;
static lv_obj_t* page_indicator = nullptr;

// Label objects
static lv_obj_t* label_name = nullptr;
static lv_obj_t* label_id = nullptr;
static lv_obj_t* label_datetime = nullptr;
static lv_obj_t* label_end_datetime = nullptr;
static lv_obj_t* label_mode = nullptr;
static lv_obj_t* label_mode_icon = nullptr;
static lv_obj_t* label_status_text = nullptr;
static lv_obj_t* label_status_icon = nullptr;
static lv_obj_t* label_items = nullptr;
static lv_obj_t* label_users = nullptr;

// Current state
static std::vector<String> auction_ids;
static int current_index = 0;
static int total_auctions = 0;
static bool has_data = false;

// Colors
#define COLOR_PRIMARY   lv_color_hex(0x2196F3)
#define COLOR_SUCCESS   lv_color_hex(0x4CAF50)
#define COLOR_DANGER    lv_color_hex(0xFF0000)
#define COLOR_WARNING   lv_color_hex(0xFF9800)
#define COLOR_DARK      lv_color_hex(0x333333)
#define COLOR_WHITE     lv_color_hex(0xFFFFFF)
#define COLOR_BORDER    lv_color_hex(0xDDDDDD)

// Icons (using Font Awesome style)
#define ICON_ARROW_UP   "\uF077"
#define ICON_ARROW_DOWN "\uF078"
#define ICON_CALENDAR   "\uF00B"
#define ICON_BOX        "\uF01C"
#define ICON_USER       "\uF07B"
#define ICON_TAG        "\uF021"
#define ICON_SETTINGS   "\uF013"
#define ICON_MODE_OPEN  "\uF06E"
#define BID_MODE        "\uF06E"  // Open mode icon

// ================== FORWARD DECLARATIONS ==================
void auction_next();
void auction_previous();
void refresh_display();

// ================== CREATE AUCTION DISPLAY ==================
void create_auction_display(lv_obj_t* parent) {
    // Configure content area
    lv_obj_set_style_pad_all(parent, 0, 0);
    lv_obj_set_style_bg_color(parent, COLOR_WHITE, 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
    
    // Create loading label
    // loading_label = lv_label_create(parent);
    // lv_label_set_text(loading_label, "Loading Auctions...");
    // lv_obj_set_style_text_color(loading_label, COLOR_DARK, 0);
    // lv_obj_set_style_text_font(loading_label, &lv_font_montserrat_14, 0);
    // lv_obj_center(loading_label);

    // ================== TOP ARROW ==================
    arrow_up = lv_label_create(parent);
    lv_label_set_text(arrow_up, ICON_ARROW_UP);
    lv_obj_set_style_text_color(arrow_up, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(arrow_up, &lv_font_montserrat_14, 0);
    lv_obj_align(arrow_up, LV_ALIGN_TOP_MID, 0, 1);
    lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);

    // ================== MAIN CARD ==================
    int card_width = 220;
    int card_height = 260;  // Slightly taller to accommodate end date

    main_card = lv_obj_create(parent);
    lv_obj_set_size(main_card, card_width, card_height);
    lv_obj_center(main_card);

    // Card styling
    lv_obj_set_style_radius(main_card, 12, 0);
    lv_obj_set_style_border_width(main_card, 2, 0);
    lv_obj_set_style_border_color(main_card, COLOR_BORDER, 0);
    lv_obj_set_style_bg_color(main_card, COLOR_WHITE, 0);
    lv_obj_set_style_shadow_width(main_card, 10, 0);
    lv_obj_set_style_shadow_color(main_card, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_shadow_ofs_x(main_card, 2, 0);
    lv_obj_set_style_shadow_ofs_y(main_card, 2, 0);
    lv_obj_set_style_pad_all(main_card, 15, 0);

    // ===== CARD CONTENT =====
    
    // Auction Name (top)
    label_name = lv_label_create(main_card);
    lv_obj_set_pos(label_name, 15, 10);
    lv_obj_set_width(label_name, card_width - 70);
    lv_obj_set_style_text_color(label_name, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(label_name, &lv_font_montserrat_14, 0);
    lv_label_set_long_mode(label_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label_name, "Auction Name");

    // Separator line 1
    lv_obj_t* line1 = lv_obj_create(main_card);
    lv_obj_set_size(line1, card_width - 30, 1);
    lv_obj_set_pos(line1, 0, 35);
    lv_obj_set_style_bg_color(line1, lv_color_hex(0xCCCCCC), 0);
    lv_obj_set_style_border_width(line1, 0, 0);
    lv_obj_set_style_pad_all(line1, 0, 0);

    // ID Icon
    lv_obj_t* ID_icon = lv_label_create(main_card);
    lv_obj_set_pos(ID_icon, 15, 45);
    lv_label_set_text(ID_icon, ICON_SETTINGS);
    lv_obj_set_style_text_color(ID_icon, COLOR_DARK, 0);
    lv_obj_set_style_text_font(ID_icon, &lv_font_montserrat_14, 0);

    // ID Label
    label_id = lv_label_create(main_card);
    lv_obj_set_pos(label_id, 40, 45);
    lv_obj_set_style_text_color(label_id, COLOR_DARK, 0);
    lv_obj_set_style_text_font(label_id, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_id, "ID: ABC123");

    // Calendar Icon for Start Date
    lv_obj_t* cal_icon = lv_label_create(main_card);
    lv_obj_set_pos(cal_icon, 15, 70);
    lv_label_set_text(cal_icon, ICON_CALENDAR);
    lv_obj_set_style_text_color(cal_icon, COLOR_DARK, 0);
    lv_obj_set_style_text_font(cal_icon, &lv_font_montserrat_14, 0);

    // Start Date/Time
    label_datetime = lv_label_create(main_card);
    lv_obj_set_pos(label_datetime, 40, 70);
    lv_obj_set_style_text_color(label_datetime, COLOR_DARK, 0);
    lv_obj_set_style_text_font(label_datetime, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_datetime, "Start: --/-- --:--");

    // Calendar Icon for End Date
    lv_obj_t* cal_icon1 = lv_label_create(main_card);
    lv_obj_set_pos(cal_icon1, 15, 95);
    lv_label_set_text(cal_icon1, ICON_CALENDAR);
    lv_obj_set_style_text_color(cal_icon1, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(cal_icon1, &lv_font_montserrat_14, 0);

    // End Date/Time (with danger color)
    label_end_datetime = lv_label_create(main_card);
    lv_obj_set_pos(label_end_datetime, 40, 95);
    lv_obj_set_style_text_color(label_end_datetime, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(label_end_datetime, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_end_datetime, "End: --/-- --:--");

    // Mode Icon
    label_mode_icon = lv_label_create(main_card);
    lv_obj_set_pos(label_mode_icon, 15, 125);
    lv_obj_set_style_text_font(label_mode_icon, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_mode_icon, BID_MODE);

    // Mode Text
    label_mode = lv_label_create(main_card);
    lv_obj_set_pos(label_mode, 40, 125);
    lv_obj_set_style_text_color(label_mode, COLOR_DARK, 0);
    lv_obj_set_style_text_font(label_mode, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_mode, "ENGLISH");

    // Status Icon
    label_status_icon = lv_label_create(main_card);
    lv_obj_set_pos(label_status_icon, 15, 155);
    lv_obj_set_style_text_font(label_status_icon, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_status_icon, ICON_TAG);

    // Status Text
    label_status_text = lv_label_create(main_card);
    lv_obj_set_pos(label_status_text, 40, 155);
    lv_obj_set_style_text_color(label_status_text, COLOR_DARK, 0);
    lv_obj_set_style_text_font(label_status_text, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_status_text, "LIVE");

    // Items Icon
    lv_obj_t* items_icon = lv_label_create(main_card);
    lv_obj_set_pos(items_icon, 15, 185);
    lv_label_set_text(items_icon, ICON_BOX);
    lv_obj_set_style_text_color(items_icon, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(items_icon, &lv_font_montserrat_14, 0);

    // Items count
    label_items = lv_label_create(main_card);
    lv_obj_set_pos(label_items, 45, 185);
    lv_obj_set_style_text_color(label_items, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(label_items, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_items, "0");

    // Items text
    lv_obj_t* items_text = lv_label_create(main_card);
    lv_obj_set_pos(items_text, 70, 185);
    lv_obj_set_style_text_color(items_text, COLOR_DARK, 0);
    lv_obj_set_style_text_font(items_text, &lv_font_montserrat_14, 0);
    lv_label_set_text(items_text, "Items");

    // Users Icon
    lv_obj_t* users_icon = lv_label_create(main_card);
    lv_obj_set_pos(users_icon, 15, 215);
    lv_label_set_text(users_icon, ICON_USER);
    lv_obj_set_style_text_color(users_icon, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(users_icon, &lv_font_montserrat_14, 0);

    // Users count
    label_users = lv_label_create(main_card);
    lv_obj_set_pos(label_users, 45, 215);
    lv_obj_set_style_text_color(label_users, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(label_users, &lv_font_montserrat_14, 0);
    lv_label_set_text(label_users, "0");

    // Users text
    lv_obj_t* users_text = lv_label_create(main_card);
    lv_obj_set_pos(users_text, 70, 215);
    lv_obj_set_style_text_color(users_text, COLOR_DARK, 0);
    lv_obj_set_style_text_font(users_text, &lv_font_montserrat_14, 0);
    lv_label_set_text(users_text, "Available");

    // ================== BOTTOM ARROW ==================
    arrow_down = lv_label_create(parent);
    lv_label_set_text(arrow_down, ICON_ARROW_DOWN);
    lv_obj_set_style_text_color(arrow_down, COLOR_DANGER, 0);
    lv_obj_set_style_text_font(arrow_down, &lv_font_montserrat_14, 0);
    lv_obj_align(arrow_down, LV_ALIGN_BOTTOM_MID, 0, -5);

    // ================== PAGE INDICATOR ==================
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
    lv_obj_add_event_cb(arrow_up, [](lv_event_t* e) { auction_previous(); }, LV_EVENT_CLICKED, nullptr);
    lv_obj_add_event_cb(arrow_down, [](lv_event_t* e) { auction_next(); }, LV_EVENT_CLICKED, nullptr);
}

// ================== INIT AUCTION SCREEN ==================
void init_auction_screen(lv_obj_t* parent) {
    // Get screen dimensions
    int screen_w = lv_obj_get_width(parent);
    int screen_h = lv_obj_get_height(parent);
    int top_bar_height = 40;
    
    // Create content area below top bar
    content_area = lv_obj_create(parent);
    lv_obj_set_size(content_area, screen_w, screen_h - top_bar_height);
    lv_obj_set_pos(content_area, 0, top_bar_height);
    lv_obj_set_style_bg_color(content_area, COLOR_WHITE, 0);
    lv_obj_set_style_border_width(content_area, 0, 0);
    lv_obj_set_style_pad_all(content_area, 0, 0);
    
    // Create the auction display inside content area
    create_auction_display(content_area);
}

// ================== LOAD AUCTIONS FROM CACHE ==================
void load_auctions_from_cache() {
    // Get auction IDs from protocol cache
    auction_ids = protocol.getAuctionIds();
    total_auctions = auction_ids.size();
    
    if (total_auctions > 0) {
        has_data = true;
        current_index = 0;
        
        // Set active auction in protocol
        if (current_index < total_auctions) {
            protocol.setActiveAuction(auction_ids[current_index]);
        }
        
        refresh_display();
       // show_auction_loading(false);
    } else {
        // No auctions in cache
        has_data = false;
        //show_auction_loading(false);
        clear_auction_screen();
    }
}

// ================== REFRESH DISPLAY ==================
void refresh_display() {
    if (!has_data || current_index >= total_auctions) {
        clear_auction_screen();
        return;
    }
    
    // Get auction data from cache
    String auctionId = auction_ids[current_index];
    AuctionData auction = protocol.getAuctionData(auctionId);
    
    if (auction.auctionId.isEmpty()) {
        clear_auction_screen();
        return;
    }
    
    // Update labels
    lv_label_set_text(label_name, auction.name.c_str());
    
    char id_buf[32];
    snprintf(id_buf, sizeof(id_buf), "ID: %s", auction.auctionId.c_str());
    lv_label_set_text(label_id, id_buf);
    
    // Format dates
    String startDate = auction.startDateTime;
    String endDate = auction.endDateTime;
    
    // Extract date and time
    String startDisplay = "Start: --/-- --:--";
    String endDisplay = "End: --/-- --:--";
    
    if (startDate.length() > 0) {
        // Format: "YYYY-MM-DD HH:MM:SS" or "YYYY-MM-DDTHH:MM:SS"
        startDisplay = "Start: ";
        if (startDate.length() >= 16) {
            // Extract MM/DD HH:MM format
            String month = startDate.substring(5, 7);
            String day = startDate.substring(8, 10);
            String time = startDate.substring(11, 16);
            startDisplay += month + "/" + day + " " + time;
        } else if (startDate.length() >= 10) {
            startDisplay += startDate.substring(0, 10);
        } else {
            startDisplay += startDate;
        }
    }
    
    if (endDate.length() > 0) {
        endDisplay = "End: ";
        if (endDate.length() >= 16) {
            String month = endDate.substring(5, 7);
            String day = endDate.substring(8, 10);
            String time = endDate.substring(11, 16);
            endDisplay += month + "/" + day + " " + time;
        } else if (endDate.length() >= 10) {
            endDisplay += endDate.substring(0, 10);
        } else {
            endDisplay += endDate;
        }
    }
    
    lv_label_set_text(label_datetime, startDisplay.c_str());
    lv_label_set_text(label_end_datetime, endDisplay.c_str());
    
    // Mode
    String modeText = auction.mode;
    if (modeText == "ENGLISH") {
        modeText = "ENGLISH";
        lv_label_set_text(label_mode_icon, "\uF06E"); // Open mode icon
    } else if (modeText == "CLOSED") {
        modeText = "CLOSED";
        lv_label_set_text(label_mode_icon, "\uF070"); // Closed mode icon
    }
    lv_label_set_text(label_mode, modeText.c_str());
    
    // Status with color
    String statusText = auction.status;
    lv_label_set_text(label_status_text, statusText.c_str());
    
    // Status color coding
    if (statusText == "ACTIVE" || statusText == "ONGOING") {
        lv_obj_set_style_text_color(label_status_text, COLOR_SUCCESS, 0);
    } else if (statusText == "UPCOMING" || statusText == "SCHEDULED") {
        lv_obj_set_style_text_color(label_status_text, COLOR_WARNING, 0);
    } else if (statusText == "ENDED" || statusText == "COMPLETED") {
        lv_obj_set_style_text_color(label_status_text, COLOR_DANGER, 0);
    } else {
        lv_obj_set_style_text_color(label_status_text, COLOR_DARK, 0);
    }
    
    // Items count
    char items_buf[16];
    snprintf(items_buf, sizeof(items_buf), "%d", auction.itemsCount);
    lv_label_set_text(label_items, items_buf);
    
    // Users count (using items count for now, can be updated later)
    char users_buf[16];
    snprintf(users_buf, sizeof(users_buf), "%d", auction.itemsCount);
    lv_label_set_text(label_users, users_buf);
    
    // Page indicator
    char page_buf[16];
    snprintf(page_buf, sizeof(page_buf), "%d/%d", current_index + 1, total_auctions);
    lv_label_set_text(page_indicator, page_buf);
    
    // Show/hide arrows
    if (total_auctions <= 1) {
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
        
        if (current_index < total_auctions - 1) {
            lv_obj_clear_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    // Show card, hide loading
    lv_obj_clear_flag(main_card, LV_OBJ_FLAG_HIDDEN);
   // lv_obj_add_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
    
    Serial.print("Displaying auction: ");
    Serial.print(auction.name.c_str());
    Serial.print(" (");
    Serial.print(current_index + 1);
    Serial.print("/");
    Serial.print(total_auctions);
    Serial.println(")");
}

// ================== AUCTION NAVIGATION ==================
void auction_next() {
    if (!has_data) return;
    if (current_index < total_auctions - 1) {
        current_index++;
        protocol.setActiveAuction(auction_ids[current_index]);
        refresh_display();
        Serial.println("Switched to next auction");
    }
}

void auction_previous() {
    if (!has_data) return;   
    if (current_index > 0) {
        current_index--;
        protocol.setActiveAuction(auction_ids[current_index]);
        refresh_display();
        Serial.println("Switched to previous auction");
    }
}
void show_auction_screen() {
    if (content_area) {
        lv_obj_clear_flag(content_area, LV_OBJ_FLAG_HIDDEN);
    }
}

void hide_auction_screen() {
    if (content_area) {
        lv_obj_add_flag(content_area, LV_OBJ_FLAG_HIDDEN);
    }
}
void auction_set_index(int index) {
    if (!has_data || index < 0 || index >= total_auctions) return;
    
    current_index = index;
    protocol.setActiveAuction(auction_ids[current_index]);
    refresh_display();
}

void auction_refresh() {
    load_auctions_from_cache();
}

// ================== SHOW/HIDE LOADING ==================
// void show_auction_loading(bool show) {
//     if (show) {
//         lv_obj_clear_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(main_card, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
//     } else {
//         lv_obj_add_flag(loading_label, LV_OBJ_FLAG_HIDDEN);
//     }
// }

// ================== CLEAR SCREEN ==================
void clear_auction_screen() {
    lv_label_set_text(label_name, "No Auctions");
    lv_label_set_text(label_id, "ID: ---");
    lv_label_set_text(label_datetime, "Start: --/-- --:--");
    lv_label_set_text(label_end_datetime, "End: --/-- --:--");
    lv_label_set_text(label_mode, "---");
    lv_label_set_text(label_status_text, "---");
    lv_label_set_text(label_items, "0");
    lv_label_set_text(label_users, "0");
    lv_obj_add_flag(arrow_up, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(arrow_down, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(page_indicator, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(main_card, LV_OBJ_FLAG_HIDDEN);
}

// ================== GET CURRENT AUCTION INFO ==================
const char* get_current_auction_id() {
    if (has_data && current_index < total_auctions) {
        return auction_ids[current_index].c_str();
    }
    return nullptr;
}

AuctionData get_current_auction() {
    if (has_data && current_index < total_auctions) {
        return protocol.getAuctionData(auction_ids[current_index]);
    }
    return AuctionData();
}

int get_current_auction_index() {
    return current_index;
}

int get_total_auctions() {
    return total_auctions;
}

bool has_auction_data() {
    return has_data;
}