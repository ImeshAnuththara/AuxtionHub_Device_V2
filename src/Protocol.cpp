// #include "Protocol.h"

// Protocol::Protocol(PubSubClient& mqttClient) {
//   _mqttClient = &mqttClient;
// }

// void Protocol::begin(String deviceId, String requestTopic, String responseTopic) {
//   _deviceId = deviceId;
//   _requestTopic = requestTopic;
//   _responseTopic = responseTopic;
// }

// void Protocol::onMessage(char* topic, byte* payload, unsigned int length) {
//   // Convert payload to string
//   String message;
//   for (unsigned int i = 0; i < length; i++) {
//     message += (char)payload[i];
//   }
  
//   Serial.println("MQTT Message received: " + message);
  
//   // Parse JSON
//   JsonDocument doc;
//   DeserializationError error = deserializeJson(doc, message);
  
//   if (error) {
//     Serial.println("JSON parse failed: " + String(error.c_str()));
//     return;
//   }
  
//   // Get action
//   String action = doc["Action"] | "";
  
//   // Handle different actions
//   if (action == "GET_AUCTION" && doc.containsKey("Auctions")) {
//     // Handle Auction response
//     JsonArray auctions = doc["Auctions"].as<JsonArray>();
//     Serial.println("\n=== AUCTIONS RECEIVED ===");
//     Serial.print("Total: ");
//     Serial.println(auctions.size());
    
//     for (JsonObject auction : auctions) {
//       Serial.print("• ");
//       Serial.print(auction["Auction_ID"] | "");
//       Serial.print(" - ");
//       Serial.print(auction["Name"] | "");
//       Serial.print(" (");
//       Serial.print(auction["Auction_Status"] | "");
//       Serial.println(")");
//     }
//     Serial.println("========================\n");
//   }
//   else if (action == "CHECK_ACCESS") {
//     // Handle Access response
//     String status = doc["Status"] | "";
    
//     if (status == "SUCCESS") {
//       printAccessResponse(doc);
//     } else {
//       printFailedResponse(doc);
//     }
//   }
//   else if (action == "GET_ITEMS") {
//     // Handle Items response
//     printItemsResponse(doc);
//   }
//   else if (action == "SUBMIT_BID") {
//     // Handle Bid response
//     printBidResponse(doc);
//   }
// }

// bool Protocol::sendGetAuction(String message_id, String datetime) {
//   if (message_id.length() == 0) {
//     message_id = generateMessageId();
//   }
  
//   if (datetime.length() == 0) {
//     datetime = getCurrentTimestamp();
//   }
  
//   currentMessageId = message_id;
  
//   JsonDocument doc;
//   doc["Message_ID"] = message_id;
//   doc["Device_ID"] = _deviceId;
//   doc["Action"] = "GET_AUCTION";
//   doc["Msg_Type"] = "request";
//   doc["DateTime"] = datetime;
  
//   String output;
//   serializeJson(doc, output);
  
//   Serial.println("Sending: " + output);
  
//   bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
//   if (success) {
//     Serial.println("✓ Auction request sent");
//   } else {
//     Serial.println("✗ Failed to send auction request");
//   }
  
//   return success;
// }

// bool Protocol::sendCheckAccess(String nfcUid, String message_id, String datetime) {
//   if (message_id.length() == 0) {
//     message_id = generateMessageId();
//   }
  
//   if (datetime.length() == 0) {
//     datetime = getCurrentTimestamp();
//   }
  
//   currentMessageId = message_id;
  
//   JsonDocument doc;
//   doc["Message_ID"] = message_id;
//   doc["Device_ID"] = _deviceId;
//   doc["Action"] = "CHECK_ACCESS";
//   doc["Msg_Type"] = "request";
//   doc["NFC_UID"] = nfcUid;
//   doc["DateTime"] = datetime;
  
//   String output;
//   serializeJson(doc, output);
  
//   Serial.println("Sending CHECK_ACCESS for UID: " + nfcUid);
//   Serial.println("Request: " + output);
  
//   bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
//   if (success) {
//     Serial.println("✓ Access check request sent");
//   } else {
//     Serial.println("✗ Failed to send access check request");
//   }
  
//   return success;
// }

// bool Protocol::sendGetItems(String auctionId, String message_id, String datetime) {
//   if (message_id.length() == 0) {
//     message_id = generateMessageId();
//   }
  
//   if (datetime.length() == 0) {
//     datetime = getCurrentTimestamp();
//   }
  
//   currentMessageId = message_id;
  
//   JsonDocument doc;
//   doc["Message_ID"] = message_id;
//   doc["Device_ID"] = _deviceId;
//   doc["Action"] = "GET_ITEMS";
//   doc["Msg_Type"] = "request";
//   doc["Auction_ID"] = auctionId;
//   doc["DateTime"] = datetime;
  
//   String output;
//   serializeJson(doc, output);
  
//   Serial.println("Sending GET_ITEMS for Auction: " + auctionId);
//   Serial.println("Request: " + output);
  
//   bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
//   if (success) {
//     Serial.println("✓ Items request sent");
//   } else {
//     Serial.println("✗ Failed to send items request");
//   }
  
//   return success;
// }

// bool Protocol::sendSubmitBid(String auctionId, String itemId, String nfcUid, float bidAmount, String currency, String message_id, String datetime) {
//   if (message_id.length() == 0) {
//     message_id = generateMessageId();
//   }
  
//   if (datetime.length() == 0) {
//     datetime = getCurrentTimestamp();
//   }
  
//   currentMessageId = message_id;
  
//   JsonDocument doc;
//   doc["Message_ID"] = message_id;
//   doc["Device_ID"] = _deviceId;
//   doc["Action"] = "SUBMIT_BID";
//   doc["Msg_Type"] = "request";
//   doc["Auction_ID"] = auctionId;
//   doc["Item_ID"] = itemId;
//   doc["NFC_UID"] = nfcUid;
//   doc["Bid_Amount"] = bidAmount;
//   doc["Currency"] = currency;
//   doc["DateTime"] = datetime;
  
//   String output;
//   serializeJson(doc, output);
  
//   Serial.println("Sending SUBMIT_BID for Item: " + itemId);
//   Serial.print("Bid Amount: ");
//   Serial.print(bidAmount);
//   Serial.print(" ");
//   Serial.println(currency);
//   Serial.println("Request: " + output);
  
//   bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
//   if (success) {
//     Serial.println("✓ Bid request sent");
//   } else {
//     Serial.println("✗ Failed to send bid request");
//   }
  
//   return success;
// }

// void Protocol::printAccessResponse(JsonDocument& doc) {
//   String nfcUid = doc["NFC_UID"] | "";
//   JsonObject access = doc["Access"];
  
//   bool granted = access["Granted"] | false;
//   String userId = access["User_ID"] | "";
//   String userName = access["User_Name"] | "";
//   String role = access["Role"] | "";
  
//   Serial.println("\n╔════════════════════════════════╗");
//   Serial.println("║     ACCESS CHECK RESPONSE      ║");
//   Serial.println("╚════════════════════════════════╝");
//   Serial.print("NFC UID: ");
//   Serial.println(nfcUid);
//   Serial.print("Status: SUCCESS");
//   Serial.println();
//   Serial.print("Access Granted: ");
//   Serial.println(granted ? "YES ✓" : "NO ✗");
  
//   if (granted) {
//     Serial.print("User ID: ");
//     Serial.println(userId);
//     Serial.print("User Name: ");
//     Serial.println(userName);
//     Serial.print("Role: ");
//     Serial.println(role);
//   }
//   Serial.println("================================\n");
// }

// void Protocol::printFailedResponse(JsonDocument& doc) {
//   String nfcUid = doc["NFC_UID"] | "";
//   String status = doc["Status"] | "";
  
//   Serial.println("\n╔════════════════════════════════╗");
//   Serial.println("║     ACCESS CHECK RESPONSE      ║");
//   Serial.println("╚════════════════════════════════╝");
//   Serial.print("NFC UID: ");
//   Serial.println(nfcUid);
//   Serial.print("Status: ");
//   Serial.println(status);
//   Serial.println("================================\n");
// }

// void Protocol::printItemsResponse(JsonDocument& doc) {
//   String auctionId = doc["Auction_ID"] | "";
//   String auctionMode = doc["Auction_Mode"] | "";
//   String auctionStatus = doc["Auction_Status"] | "";
//   String status = doc["Status"] | "";
//   int itemsCount = doc["Items_Count"] | 0;
//   JsonArray items = doc["Items"].as<JsonArray>();
  
//   Serial.println("\n╔═══════════════════════════════════════╗");
//   Serial.println("║         ITEMS RECEIVED                ║");
//   Serial.println("╚═══════════════════════════════════════╝");
//   Serial.print("Auction ID: ");
//   Serial.println(auctionId);
//   Serial.print("Auction Mode: ");
//   Serial.println(auctionMode);
//   Serial.print("Auction Status: ");
//   Serial.println(auctionStatus);
//   Serial.print("Status: ");
//   Serial.println(status);
//   Serial.print("Total Items: ");
//   Serial.println(itemsCount);
//   Serial.println("---------------------------------------");
  
//   int itemNum = 1;
//   for (JsonObject item : items) {
//     Serial.print("\nItem ");
//     Serial.print(itemNum);
//     Serial.println(":");
//     Serial.print("  Item ID: ");
//     Serial.println(item["Item_ID"] | "");
//     Serial.print("  Name: ");
//     Serial.println(item["Name"] | "");
//     Serial.print("  Status: ");
//     Serial.println(item["Status"] | "");
//     Serial.print("  Current Price: ");
//     Serial.print(item["Current_Price"] | 0.00);
//     Serial.print(" ");
//     Serial.println(item["Currency"] | "");
    
//     if (auctionMode == "ENGLISH") {
//       Serial.print("  Next Min Bid: ");
//       Serial.print(item["Next_Min_Bid"] | 0.00);
//       Serial.println( (item["Currency"] | ""));
//     }
    
//     Serial.print("  End DateTime: ");
//     Serial.println(item["End_DateTime"] | "");
//     Serial.print("  Remaining Seconds: ");
//     Serial.println(item["Remaining_Seconds"] | 0);
    
//     if (auctionMode == "CLOSED") {
//       bool bidSubmitted = item["Your_Bid_Submitted"] | false;
//       Serial.print("  Your Bid Submitted: ");
//       Serial.println(bidSubmitted ? "YES ✓" : "NO ✗");
//     }
    
//     itemNum++;
//   }
  
//   Serial.println("\n========================================\n");
// }

// void Protocol::printBidResponse(JsonDocument& doc) {
//   String status = doc["Status"] | "";
//   String bidStatus = doc["Bid_Status"] | "";
//   String auctionId = doc["Auction_ID"] | "";
//   String auctionMode = doc["Auction_Mode"] | "";
//   String itemId = doc["Item_ID"] | "";
//   String nfcUid = doc["NFC_UID"] | "";
  
//   Serial.println("\n╔════════════════════════════════╗");
//   Serial.println("║        BID RESPONSE            ║");
//   Serial.println("╚════════════════════════════════╝");
//   Serial.print("Auction ID: ");
//   Serial.println(auctionId);
//   Serial.print("Auction Mode: ");
//   Serial.println(auctionMode);
//   Serial.print("Item ID: ");
//   Serial.println(itemId);
//   Serial.print("NFC UID: ");
//   Serial.println(nfcUid);
//   Serial.print("Status: ");
//   Serial.println(status);
//   Serial.print("Bid Status: ");
//   Serial.println(bidStatus);
  
//   if (status == "SUCCESS" && bidStatus == "ACCEPTED") {
//     float currentHighestBid = doc["Current_Highest_Bid"] | 0.00;
//     String currency = doc["Currency"] | "LKR";
//     float nextMinBid = doc["Next_Min_Bid"] | 0.00;
    
//     Serial.print("Current Highest Bid: ");
//     Serial.print(currentHighestBid);
//     Serial.print(" ");
//     Serial.println(currency);
//     Serial.print("Next Minimum Bid: ");
//     Serial.print(nextMinBid);
//     Serial.print(" ");
//     Serial.println(currency);
//     Serial.println("✓ Bid ACCEPTED!");
//   } 
//   else if (status == "FAILED" && bidStatus == "REJECTED") {
//     int reason = doc["Reason"] | 0;
//     Serial.print("Reason Code: ");
//     Serial.println(reason);
//     Serial.println("✗ Bid REJECTED!");
//   }
  
//   Serial.println("================================\n");
// }

// String Protocol::generateMessageId() {
//   unsigned long timestamp = millis();
//   int randomNum = random(1000, 9999);
//   return "MSG_" + String(timestamp) + "_" + String(randomNum);
// }

// String Protocol::getCurrentTimestamp() {
//   struct tm timeinfo;
//     // Try to get real time
//     if (getLocalTime(&timeinfo, 1000)) {
//         char buffer[30];
//         strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
//         return String(buffer);
//     }
// }


#include "Protocol.h"

Protocol::Protocol(PubSubClient& mqttClient) {
  _mqttClient = &mqttClient;
}

void Protocol::begin(String deviceId, String requestTopic, String responseTopic) {
  _deviceId = deviceId;
  _requestTopic = requestTopic;
  _responseTopic = responseTopic;
  
  // Initialize cache maps
  _auctions.clear();
  _items.clear();
  _activeAuctionId = "";
  _activeItemId = "";
}

void Protocol::onMessage(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println("MQTT Message received: " + message);
  
  // Parse JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);
  
  if (error) {
    Serial.println("JSON parse failed: " + String(error.c_str()));
    return;
  }
  
  // Get action
  String action = doc["Action"] | "";
  
  // Handle different actions
  if (action == "GET_AUCTION" && doc.containsKey("Auctions")) {
    handleAuctionsResponse(doc);
  }
  else if (action == "CHECK_ACCESS") {
    handleAccessResponse(doc);
  }
  else if (action == "GET_ITEMS") {
    handleItemsResponse(doc);
  }
  else if (action == "SUBMIT_BID") {
    handleBidResponse(doc);
  }
}

// ============= CACHE MANAGEMENT METHODS =============

void Protocol::handleAuctionsResponse(JsonDocument& doc) {
  // Clear existing auctions
  _auctions.clear();
  
  JsonArray auctions = doc["Auctions"].as<JsonArray>();
  Serial.println("\n=== AUCTIONS RECEIVED & CACHED ===");
  Serial.print("Total: ");
  Serial.println(auctions.size());
  
  for (JsonObject auction : auctions) {
    AuctionData auctionData;
    auctionData.auctionId = auction["Auction_ID"] | "";
    auctionData.name = auction["Name"] | "";
    auctionData.status = auction["Auction_Status"] | "";
    auctionData.mode = auction["Auction_Mode"] | "";
    auctionData.startDateTime = auction["Start_DateTime"] | "";
    auctionData.endDateTime = auction["End_DateTime"] | "";
    auctionData.description = auction["Description"] | "";
    auctionData.itemsCount = auction["Items_Count"] | 0;
    
    // Store in cache
    _auctions[auctionData.auctionId] = auctionData;
    
    Serial.print("• ");
    Serial.print(auctionData.auctionId);
    Serial.print(" - ");
    Serial.print(auctionData.name);
    Serial.print(" (");
    Serial.print(auctionData.status);
    Serial.print(") Items: ");
    Serial.println(auctionData.itemsCount);
  }
  
  // Notify callback if registered
  if (_auctionsCallback) {
    _auctionsCallback();
  }
  
  Serial.println("================================\n");
}

void Protocol::handleItemsResponse(JsonDocument& doc) {
  String auctionId = doc["Auction_ID"] | "";
  String auctionMode = doc["Auction_Mode"] | "";
  String auctionStatus = doc["Auction_Status"] | "";
  
  // Clear existing items for this auction
  if (_items.find(auctionId) != _items.end()) {
    _items[auctionId].clear();
  } else {
    _items[auctionId] = std::map<String, ItemData>();
  }
  
  JsonArray items = doc["Items"].as<JsonArray>();
  
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║      ITEMS RECEIVED & CACHED         ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.print("Auction ID: ");
  Serial.println(auctionId);
  Serial.print("Auction Mode: ");
  Serial.println(auctionMode);
  Serial.print("Total Items: ");
  Serial.println(items.size());
  Serial.println("---------------------------------------");
  
  for (JsonObject item : items) {
    ItemData itemData;
    itemData.itemId = item["Item_ID"] | "";
    itemData.name = item["Name"] | "";
    itemData.description = item["Description"] | "";
    itemData.status = item["Status"] | "";
    itemData.currentPrice = item["Current_Price"] | 0.00;
    itemData.currency = item["Currency"] | "LKR";
    itemData.startPrice = item["Start_Price"] | 0.00;
    itemData.nextMinBid = item["Next_Min_Bid"] | 0.00;
    itemData.endDateTime = item["End_DateTime"] | "";
    itemData.remainingSeconds = item["Remaining_Seconds"] | 0;
    itemData.yourBidSubmitted = item["Your_Bid_Submitted"] | false;
    itemData.yourBidAmount = item["Your_Bid_Amount"] | 0.00;
    itemData.highestBidder = item["Highest_Bidder"] | "";
    itemData.bidCount = item["Bid_Count"] | 0;
    
    // Store in cache
    _items[auctionId][itemData.itemId] = itemData;
    
    Serial.print("\nItem: ");
    Serial.println(itemData.name);
    Serial.print("  ID: ");
    Serial.println(itemData.itemId);
    Serial.print("  Price: ");
    Serial.print(itemData.currentPrice);
    Serial.print(" ");
    Serial.println(itemData.currency);
  }
  
  // Set as active auction if not set
  if (_activeAuctionId.isEmpty()) {
    _activeAuctionId = auctionId;
  }
  
  // Notify callback if registered
  if (_itemsCallback) {
    _itemsCallback(auctionId);
  }
  
  Serial.println("\n========================================\n");
}

void Protocol::handleBidResponse(JsonDocument& doc) {
  String status = doc["Status"] | "";
  String bidStatus = doc["Bid_Status"] | "";
  String auctionId = doc["Auction_ID"] | "";
  String itemId = doc["Item_ID"] | "";
  String nfcUid = doc["NFC_UID"] | "";
  
  // Update cache if bid was accepted
  if (status == "SUCCESS" && bidStatus == "ACCEPTED") {
    // Update the item in cache with new bid information
    if (_items.find(auctionId) != _items.end() && 
        _items[auctionId].find(itemId) != _items[auctionId].end()) {
      
      _items[auctionId][itemId].currentPrice = doc["Current_Highest_Bid"] | 0.00;
      _items[auctionId][itemId].nextMinBid = doc["Next_Min_Bid"] | 0.00;
      _items[auctionId][itemId].yourBidSubmitted = true;
      _items[auctionId][itemId].yourBidAmount = doc["Bid_Amount"] | 0.00;
      
      Serial.println("✓ Cache updated with new bid information");
    }
  }
  
  // Notify callback if registered
  if (_bidCallback) {
    _bidCallback(auctionId, itemId, status == "SUCCESS" && bidStatus == "ACCEPTED");
  }
  
  printBidResponse(doc);
}

void Protocol::handleAccessResponse(JsonDocument& doc) {
  // Store access info in current session
  _currentAccessGranted = doc["Access"]["Granted"] | false;
  _currentUserId = doc["Access"]["User_ID"] | "";
  _currentUserName = doc["Access"]["User_Name"] | "";
  _currentUserRole = doc["Access"]["Role"] | "";
  _currentNfcUid = doc["NFC_UID"] | "";
  
  // Notify callback if registered
  if (_accessCallback) {
    _accessCallback(_currentAccessGranted, _currentUserId, _currentUserName);
  }
  
  printAccessResponse(doc);
}

// ============= PUBLIC CACHE ACCESS METHODS =============

std::vector<String> Protocol::getAuctionIds() {
  std::vector<String> auctionIds;
  for (auto& pair : _auctions) {
    auctionIds.push_back(pair.first);
  }
  return auctionIds;
}

std::vector<String> Protocol::getActiveAuctions() {
  std::vector<String> activeAuctions;
  for (auto& pair : _auctions) {
    if (pair.second.status == "ACTIVE" || pair.second.status == "ONGOING") {
      activeAuctions.push_back(pair.first);
    }
  }
  return activeAuctions;
}

AuctionData Protocol::getAuctionData(String auctionId) {
  if (_auctions.find(auctionId) != _auctions.end()) {
    return _auctions[auctionId];
  }
  return AuctionData(); // Return empty struct if not found
}

std::vector<ItemData> Protocol::getItemsForAuction(String auctionId) {
  std::vector<ItemData> items;
  if (_items.find(auctionId) != _items.end()) {
    for (auto& pair : _items[auctionId]) {
      items.push_back(pair.second);
    }
  }
  return items;
}

ItemData Protocol::getItemData(String auctionId, String itemId) {
  if (_items.find(auctionId) != _items.end() && 
      _items[auctionId].find(itemId) != _items[auctionId].end()) {
    return _items[auctionId][itemId];
  }
  return ItemData(); // Return empty struct if not found
}

bool Protocol::hasAuction(String auctionId) {
  return _auctions.find(auctionId) != _auctions.end();
}

bool Protocol::hasItem(String auctionId, String itemId) {
  return (_items.find(auctionId) != _items.end() && 
          _items[auctionId].find(itemId) != _items[auctionId].end());
}

void Protocol::setActiveAuction(String auctionId) {
  if (hasAuction(auctionId)) {
    _activeAuctionId = auctionId;
    Serial.println("Active auction set to: " + auctionId);
  }
}

String Protocol::getActiveAuction() {
  return _activeAuctionId;
}

void Protocol::setActiveItem(String itemId) {
  _activeItemId = itemId;
}

String Protocol::getActiveItem() {
  return _activeItemId;
}

int Protocol::getTotalAuctions() {
  return _auctions.size();
}

int Protocol::getTotalItemsForAuction(String auctionId) {
  if (_items.find(auctionId) != _items.end()) {
    return _items[auctionId].size();
  }
  return 0;
}

AuctionData Protocol::getNextAuction(String currentAuctionId) {
  std::vector<String> ids = getAuctionIds();
  for (size_t i = 0; i < ids.size(); i++) {
    if (ids[i] == currentAuctionId && i + 1 < ids.size()) {
      return _auctions[ids[i + 1]];
    }
  }
  return AuctionData();
}

AuctionData Protocol::getPreviousAuction(String currentAuctionId) {
  std::vector<String> ids = getAuctionIds();
  for (size_t i = 0; i < ids.size(); i++) {
    if (ids[i] == currentAuctionId && i > 0) {
      return _auctions[ids[i - 1]];
    }
  }
  return AuctionData();
}

ItemData Protocol::getNextItem(String auctionId, String currentItemId) {
  std::vector<ItemData> items = getItemsForAuction(auctionId);
  for (size_t i = 0; i < items.size(); i++) {
    if (items[i].itemId == currentItemId && i + 1 < items.size()) {
      return items[i + 1];
    }
  }
  return ItemData();
}

ItemData Protocol::getPreviousItem(String auctionId, String currentItemId) {
  std::vector<ItemData> items = getItemsForAuction(auctionId);
  for (size_t i = 0; i < items.size(); i++) {
    if (items[i].itemId == currentItemId && i > 0) {
      return items[i - 1];
    }
  }
  return ItemData();
}

// ============= ACCESS INFO METHODS =============

bool Protocol::isAccessGranted() {
  return _currentAccessGranted;
}

String Protocol::getCurrentUserId() {
  return _currentUserId;
}

String Protocol::getCurrentUserName() {
  return _currentUserName;
}

String Protocol::getCurrentUserRole() {
  return _currentUserRole;
}

String Protocol::getCurrentNfcUid() {
  return _currentNfcUid;
}

// ============= CALLBACK REGISTRATION =============

void Protocol::setAuctionsCallback(std::function<void()> callback) {
  _auctionsCallback = callback;
}

void Protocol::setItemsCallback(std::function<void(String)> callback) {
  _itemsCallback = callback;
}

void Protocol::setBidCallback(std::function<void(String, String, bool)> callback) {
  _bidCallback = callback;
}

void Protocol::setAccessCallback(std::function<void(bool, String, String)> callback) {
  _accessCallback = callback;
}

// ============= EXISTING SEND METHODS =============

bool Protocol::sendGetAuction(String message_id, String datetime) {
  if (message_id.length() == 0) {
    message_id = generateMessageId();
  }
  
  if (datetime.length() == 0) {
    datetime = getCurrentTimestamp();
  }
  
  currentMessageId = message_id;
  
  JsonDocument doc;
  doc["Message_ID"] = message_id;
  doc["Device_ID"] = _deviceId;
  doc["Action"] = "GET_AUCTION";
  doc["Msg_Type"] = "request";
  doc["DateTime"] = datetime;
  
  String output;
  serializeJson(doc, output);
  
  Serial.println("Sending: " + output);
  
  bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
  if (success) {
    Serial.println("✓ Auction request sent");
  } else {
    Serial.println("✗ Failed to send auction request");
  }
  
  return success;
}

bool Protocol::sendCheckAccess(String nfcUid, String message_id, String datetime) {
  if (message_id.length() == 0) {
    message_id = generateMessageId();
  }
  
  if (datetime.length() == 0) {
    datetime = getCurrentTimestamp();
  }
  
  currentMessageId = message_id;
  
  JsonDocument doc;
  doc["Message_ID"] = message_id;
  doc["Device_ID"] = _deviceId;
  doc["Action"] = "CHECK_ACCESS";
  doc["Msg_Type"] = "request";
  doc["NFC_UID"] = nfcUid;
  doc["DateTime"] = datetime;
  
  String output;
  serializeJson(doc, output);
  
  Serial.println("Sending CHECK_ACCESS for UID: " + nfcUid);
  Serial.println("Request: " + output);
  
  bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
  if (success) {
    Serial.println("✓ Access check request sent");
  } else {
    Serial.println("✗ Failed to send access check request");
  }
  
  return success;
}

bool Protocol::sendGetItems(String auctionId, String message_id, String datetime) {
  if (message_id.length() == 0) {
    message_id = generateMessageId();
  }
  
  if (datetime.length() == 0) {
    datetime = getCurrentTimestamp();
  }
  
  currentMessageId = message_id;
  
  JsonDocument doc;
  doc["Message_ID"] = message_id;
  doc["Device_ID"] = _deviceId;
  doc["Action"] = "GET_ITEMS";
  doc["Msg_Type"] = "request";
  doc["Auction_ID"] = auctionId;
  doc["DateTime"] = datetime;
  
  String output;
  serializeJson(doc, output);
  
  Serial.println("Sending GET_ITEMS for Auction: " + auctionId);
  Serial.println("Request: " + output);
  
  bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
  if (success) {
    Serial.println("✓ Items request sent");
  } else {
    Serial.println("✗ Failed to send items request");
  }
  
  return success;
}

bool Protocol::sendSubmitBid(String auctionId, String itemId, String nfcUid, float bidAmount, String currency, String message_id, String datetime) {
  if (message_id.length() == 0) {
    message_id = generateMessageId();
  }
  
  if (datetime.length() == 0) {
    datetime = getCurrentTimestamp();
  }
  
  currentMessageId = message_id;
  
  JsonDocument doc;
  doc["Message_ID"] = message_id;
  doc["Device_ID"] = _deviceId;
  doc["Action"] = "SUBMIT_BID";
  doc["Msg_Type"] = "request";
  doc["Auction_ID"] = auctionId;
  doc["Item_ID"] = itemId;
  doc["NFC_UID"] = nfcUid;
  doc["Bid_Amount"] = bidAmount;
  doc["Currency"] = currency;
  doc["DateTime"] = datetime;
  
  String output;
  serializeJson(doc, output);
  
  Serial.println("Sending SUBMIT_BID for Item: " + itemId);
  Serial.print("Bid Amount: ");
  Serial.print(bidAmount);
  Serial.print(" ");
  Serial.println(currency);
  Serial.println("Request: " + output);
  
  bool success = _mqttClient->publish(_requestTopic.c_str(), output.c_str());
  
  if (success) {
    Serial.println("✓ Bid request sent");
  } else {
    Serial.println("✗ Failed to send bid request");
  }
  
  return success;
}

// ============= PRINT METHODS =============

void Protocol::printAccessResponse(JsonDocument& doc) {
  String nfcUid = doc["NFC_UID"] | "";
  JsonObject access = doc["Access"];
  
  bool granted = access["Granted"] | false;
  String userId = access["User_ID"] | "";
  String userName = access["User_Name"] | "";
  String role = access["Role"] | "";
  
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║     ACCESS CHECK RESPONSE      ║");
  Serial.println("╚════════════════════════════════╝");
  Serial.print("NFC UID: ");
  Serial.println(nfcUid);
  Serial.print("Status: SUCCESS");
  Serial.println();
  Serial.print("Access Granted: ");
  Serial.println(granted ? "YES ✓" : "NO ✗");
  
  if (granted) {
    Serial.print("User ID: ");
    Serial.println(userId);
    Serial.print("User Name: ");
    Serial.println(userName);
    Serial.print("Role: ");
    Serial.println(role);
  }
  Serial.println("================================\n");
}

void Protocol::printFailedResponse(JsonDocument& doc) {
  String nfcUid = doc["NFC_UID"] | "";
  String status = doc["Status"] | "";
  
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║     ACCESS CHECK RESPONSE      ║");
  Serial.println("╚════════════════════════════════╝");
  Serial.print("NFC UID: ");
  Serial.println(nfcUid);
  Serial.print("Status: ");
  Serial.println(status);
  Serial.println("================================\n");
}

void Protocol::printItemsResponse(JsonDocument& doc) {
  String auctionId = doc["Auction_ID"] | "";
  String auctionMode = doc["Auction_Mode"] | "";
  String auctionStatus = doc["Auction_Status"] | "";
  String status = doc["Status"] | "";
  int itemsCount = doc["Items_Count"] | 0;
  JsonArray items = doc["Items"].as<JsonArray>();
  
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║         ITEMS RECEIVED                ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.print("Auction ID: ");
  Serial.println(auctionId);
  Serial.print("Auction Mode: ");
  Serial.println(auctionMode);
  Serial.print("Auction Status: ");
  Serial.println(auctionStatus);
  Serial.print("Status: ");
  Serial.println(status);
  Serial.print("Total Items: ");
  Serial.println(itemsCount);
  Serial.println("---------------------------------------");
  
  int itemNum = 1;
  for (JsonObject item : items) {
    Serial.print("\nItem ");
    Serial.print(itemNum);
    Serial.println(":");
    Serial.print("  Item ID: ");
    Serial.println(item["Item_ID"] | "");
    Serial.print("  Name: ");
    Serial.println(item["Name"] | "");
    Serial.print("  Status: ");
    Serial.println(item["Status"] | "");
    Serial.print("  Current Price: ");
    Serial.print(item["Current_Price"] | 0.00);
    Serial.print(" ");
    Serial.println(item["Currency"] | "");
    
    if (auctionMode == "ENGLISH") {
      Serial.print("  Next Min Bid: ");
      Serial.print(item["Next_Min_Bid"] | 0.00);
      Serial.println( (item["Currency"] | ""));
    }
    
    Serial.print("  End DateTime: ");
    Serial.println(item["End_DateTime"] | "");
    Serial.print("  Remaining Seconds: ");
    Serial.println(item["Remaining_Seconds"] | 0);
    
    if (auctionMode == "CLOSED") {
      bool bidSubmitted = item["Your_Bid_Submitted"] | false;
      Serial.print("  Your Bid Submitted: ");
      Serial.println(bidSubmitted ? "YES ✓" : "NO ✗");
    }
    
    itemNum++;
  }
  
  Serial.println("\n========================================\n");
}

void Protocol::printBidResponse(JsonDocument& doc) {
  String status = doc["Status"] | "";
  String bidStatus = doc["Bid_Status"] | "";
  String auctionId = doc["Auction_ID"] | "";
  String auctionMode = doc["Auction_Mode"] | "";
  String itemId = doc["Item_ID"] | "";
  String nfcUid = doc["NFC_UID"] | "";
  
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║        BID RESPONSE            ║");
  Serial.println("╚════════════════════════════════╝");
  Serial.print("Auction ID: ");
  Serial.println(auctionId);
  Serial.print("Auction Mode: ");
  Serial.println(auctionMode);
  Serial.print("Item ID: ");
  Serial.println(itemId);
  Serial.print("NFC UID: ");
  Serial.println(nfcUid);
  Serial.print("Status: ");
  Serial.println(status);
  Serial.print("Bid Status: ");
  Serial.println(bidStatus);
  
  if (status == "SUCCESS" && bidStatus == "ACCEPTED") {
    float currentHighestBid = doc["Current_Highest_Bid"] | 0.00;
    String currency = doc["Currency"] | "LKR";
    float nextMinBid = doc["Next_Min_Bid"] | 0.00;
    
    Serial.print("Current Highest Bid: ");
    Serial.print(currentHighestBid);
    Serial.print(" ");
    Serial.println(currency);
    Serial.print("Next Minimum Bid: ");
    Serial.print(nextMinBid);
    Serial.print(" ");
    Serial.println(currency);
    Serial.println("✓ Bid ACCEPTED!");
  } 
  else if (status == "FAILED" && bidStatus == "REJECTED") {
    int reason = doc["Reason"] | 0;
    Serial.print("Reason Code: ");
    Serial.println(reason);
    Serial.println("✗ Bid REJECTED!");
  }
  
  Serial.println("================================\n");
}

String Protocol::generateMessageId() {
  unsigned long timestamp = millis();
  int randomNum = random(1000, 9999);
  return "MSG_" + String(timestamp) + "_" + String(randomNum);
}

String Protocol::getCurrentTimestamp() {
  struct tm timeinfo;
  // Try to get real time
  if (getLocalTime(&timeinfo, 1000)) {
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    return String(buffer);
  }
  return "";
}