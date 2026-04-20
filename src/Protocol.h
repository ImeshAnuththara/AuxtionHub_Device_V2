// #ifndef PROTOCOL_H
// #define PROTOCOL_H

// #include <Arduino.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>

// class Protocol {
//   public:
//     Protocol(PubSubClient& mqttClient);
    
//     void begin(String deviceId, String requestTopic, String responseTopic);
//     void onMessage(char* topic, byte* payload, unsigned int length);
    
//     // Send GET_AUCTION request
//     bool sendGetAuction(String message_id = "", String datetime = "");
    
//     // Send CHECK_ACCESS request with NFC UID
//     bool sendCheckAccess(String nfcUid, String message_id = "", String datetime = "");
    
//     // Send GET_ITEMS request with Auction ID
//     bool sendGetItems(String auctionId, String message_id = "", String datetime = "");
    
//     // Get current message ID for tracking
//     String getCurrentMessageId() { return currentMessageId; }
    
//   private:
//     PubSubClient* _mqttClient;
//     String _deviceId;
//     String _requestTopic;
//     String _responseTopic;
//     String currentMessageId;
    
//     String generateMessageId();
//     String getCurrentTimestamp();
//     void printAccessResponse(JsonDocument& doc);
//     void printFailedResponse(JsonDocument& doc);
//     void printItemsResponse(JsonDocument& doc);
// };

// #endif

// #ifndef PROTOCOL_H
// #define PROTOCOL_H

// #include <Arduino.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>
// #include <time.h>

// class Protocol {
//   public:
//     Protocol(PubSubClient& mqttClient);
    
//     void begin(String deviceId, String requestTopic, String responseTopic);
//     void onMessage(char* topic, byte* payload, unsigned int length);
    
//     // Send GET_AUCTION request
//     bool sendGetAuction(String message_id = "", String datetime = "");
    
//     // Send CHECK_ACCESS request with NFC UID
//     bool sendCheckAccess(String nfcUid, String message_id = "", String datetime = "");
    
//     // Send GET_ITEMS request with Auction ID
//     bool sendGetItems(String auctionId, String message_id = "", String datetime = "");
    
//     // Send SUBMIT_BID request
//     bool sendSubmitBid(String auctionId, String itemId, String nfcUid, float bidAmount, String currency = "LKR", String message_id = "", String datetime = "");
    
//     // Get current message ID for tracking
//     String getCurrentMessageId() { return currentMessageId; }
    
//   private:
//     PubSubClient* _mqttClient;
//     String _deviceId;
//     String _requestTopic;
//     String _responseTopic;
//     String currentMessageId;
    
//     String generateMessageId();
//     String getCurrentTimestamp();
//     void printAccessResponse(JsonDocument& doc);
//     void printFailedResponse(JsonDocument& doc);
//     void printItemsResponse(JsonDocument& doc);
//     void printBidResponse(JsonDocument& doc);
// };

// #endif

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <map>
#include <vector>
#include <functional>

// Data structure for Auction
struct AuctionData {
  String auctionId;
  String name;
  String status;
  String mode;
  String startDateTime;
  String endDateTime;
  String description;
  int itemsCount;
  
  AuctionData() : itemsCount(0) {}
};

// Data structure for Item
struct ItemData {
  String itemId;
  String name;
  String description;
  String status;
  float currentPrice;
  String currency;
  float startPrice;
  float nextMinBid;
  String endDateTime;
  int remainingSeconds;
  bool yourBidSubmitted;
  float yourBidAmount;
  String highestBidder;
  int bidCount;
  
  ItemData() : currentPrice(0), startPrice(0), nextMinBid(0), 
               remainingSeconds(0), yourBidSubmitted(false), 
               yourBidAmount(0), bidCount(0) {}
};

class Protocol {
public:
  Protocol(PubSubClient& mqttClient);
  
  void begin(String deviceId, String requestTopic, String responseTopic);
  void onMessage(char* topic, byte* payload, unsigned int length);
  
  // Send methods
  bool sendGetAuction(String message_id = "", String datetime = "");
  bool sendCheckAccess(String nfcUid, String message_id = "", String datetime = "");
  bool sendGetItems(String auctionId, String message_id = "", String datetime = "");
  bool sendSubmitBid(String auctionId, String itemId, String nfcUid, 
                     float bidAmount, String currency = "LKR", 
                     String message_id = "", String datetime = "");
  
  // Cache access methods
  std::vector<String> getAuctionIds();
  std::vector<String> getActiveAuctions();
  AuctionData getAuctionData(String auctionId);
  std::vector<ItemData> getItemsForAuction(String auctionId);
  ItemData getItemData(String auctionId, String itemId);
  bool hasAuction(String auctionId);
  bool hasItem(String auctionId, String itemId);
  
  // Navigation helpers
  void setActiveAuction(String auctionId);
  String getActiveAuction();
  void setActiveItem(String itemId);
  String getActiveItem();
  int getTotalAuctions();
  int getTotalItemsForAuction(String auctionId);
  AuctionData getNextAuction(String currentAuctionId);
  AuctionData getPreviousAuction(String currentAuctionId);
  ItemData getNextItem(String auctionId, String currentItemId);
  ItemData getPreviousItem(String auctionId, String currentItemId);
  
  // Access info
  bool isAccessGranted();
  String getCurrentUserId();
  String getCurrentUserName();
  String getCurrentUserRole();
  String getCurrentNfcUid();
  
  // Callback registration
  void setAuctionsCallback(std::function<void()> callback);
  void setItemsCallback(std::function<void(String)> callback);
  void setBidCallback(std::function<void(String, String, bool)> callback);
  void setAccessCallback(std::function<void(bool, String, String)> callback);
  
private:
  PubSubClient* _mqttClient;
  String _deviceId;
  String _requestTopic;
  String _responseTopic;
  String currentMessageId;
  
  // Cache storage
  std::map<String, AuctionData> _auctions;           // auctionId -> AuctionData
  std::map<String, std::map<String, ItemData>> _items; // auctionId -> (itemId -> ItemData)
  
  // Current session
  String _activeAuctionId;
  String _activeItemId;
  
  // Access info
  bool _currentAccessGranted;
  String _currentUserId;
  String _currentUserName;
  String _currentUserRole;
  String _currentNfcUid;
  
  // Callbacks
  std::function<void()> _auctionsCallback;
  std::function<void(String)> _itemsCallback;
  std::function<void(String, String, bool)> _bidCallback;
  std::function<void(bool, String, String)> _accessCallback;
  
  // Message handlers
  void handleAuctionsResponse(JsonDocument& doc);
  void handleItemsResponse(JsonDocument& doc);
  void handleBidResponse(JsonDocument& doc);
  void handleAccessResponse(JsonDocument& doc);
  
  // Print methods
  void printAccessResponse(JsonDocument& doc);
  void printFailedResponse(JsonDocument& doc);
  void printItemsResponse(JsonDocument& doc);
  void printBidResponse(JsonDocument& doc);
  
  // Helper methods
  String generateMessageId();
  String getCurrentTimestamp();
};

#endif