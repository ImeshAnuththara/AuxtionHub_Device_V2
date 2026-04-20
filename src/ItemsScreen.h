#ifndef ITEMS_SCREEN_H
#define ITEMS_SCREEN_H

#include <lvgl.h>
#include <Arduino.h>
#include "Protocol.h"

// Store items screen content area (extern for main code access)
extern lv_obj_t* items_content_area;
// Initialize items screen
void init_items_screen(lv_obj_t* parent);
// Load items from cache for a specific auction
void load_items_for_auction(String auctionId);
// Navigation functions
void item_next();
void item_previous();
// Display control
void show_items_loading(bool show);
void clear_items_screen();
void hide_items_screen();
void show_items_screen();

// Get current item info
const char* get_current_item_id();
ItemData get_current_item();
int get_current_item_index();
int get_total_items();
bool has_items_data();

#endif