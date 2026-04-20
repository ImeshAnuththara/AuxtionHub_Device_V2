#ifndef AUCTION_SCREEN_H
#define AUCTION_SCREEN_H

#include <lvgl.h>
#include <Arduino.h>
#include "Protocol.h"

// Initialize auction screen
void init_auction_screen(lv_obj_t* parent);

// Load auctions from protocol cache
void load_auctions_from_cache();

// Refresh current display
void refresh_display();

// Navigation functions
void auction_next();
void auction_previous();
void auction_set_index(int index);
void auction_refresh();

// Loading states
void show_auction_loading(bool show);
void show_auction_error(const char* message);
void show_auction_screen();
void hide_auction_screen();

// Clear display
void clear_auction_screen();

// Get current auction info
const char* get_current_auction_id();
AuctionData get_current_auction();
int get_current_auction_index();
int get_total_auctions();
bool has_auction_data();

#endif