// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <logo.h>
// #include <PubSubClient.h>
// #include <LVGLScreen.h>
// #include <tft_init.h>
// #include <Hardware.h>
// #include <Preferences.h>
// #include <apmode.h>
// #include <Button.h>
// #include <Secret.h>
// #include <ArduinoJson.h>
// #include <Protocol.h>
// #include <Header_template.h>
// #include <AuctionScreen.h>
// #include <ItemsScreen.h>

// // ---------------- TFT ----------------
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   5
// #define TFT_DC   21
// #define TFT_RST  22

// //SD Card
// #define SD_MISO 19
// #define SD_CS   13
// #define SD_BTN  15

// #define TFT_WIDTH  240
// #define TFT_HEIGHT 320
// #define LOGO_WIDTH 240
// #define LOGO_HEIGHT 80

// // ---------------- BUTTONS ----------------
// #define BTN1 26
// #define BTN2 27
// #define BTN3 25
// #define BTN4 32
// #define BTN5 33

// // ---------------- I2C ----------------
// #define SDA_PIN 16
// #define SCL_PIN 17
// #define POWER_PIN 12
// #define MAX17048_ADDR 0x36

// // ------------------ MQTT ------------------
// const char* AUCTION_REQ_TOPIC = "auction/Device_001/request";
// const char* AUCTION_RES_TOPIC = "auction/Device_001/response";
// const char* awsEndpoint = "a1m322vfibs32e-ats.iot.ap-south-1.amazonaws.com";
// String selectedAuctionId; // For testing, this will be set when user selects an auction

// // ------------------ GLOBALS ------------------
// String deviceId = "Device_001";
// String clientId;
// String firmwareVersion = "1.0.0";
// String hardwareVersion = "1.0";

// // ---------------- SYSTEM ----------------
// WiFiClientSecure net;
// PubSubClient mqttClient(net);
// APMode apMode("AuctionHub", "12345678");
// Preferences preferences;

// // Keypad
// const uint8_t rowPins[5] = {0,1,2,3,4};
// const uint8_t colPins[3] = {5,6,7};
// const uint8_t ledPins[] = {8,9,10,11,12,13,14,15}; // MCP pins for LEDs
// char keys[5][3] = {
//   {'1','2','3'},
//   {'4','5','6'},
//   {'7','8','9'},
//   {'*','0','#'},
//   {'A','B','C'}
// };
// TFT_init tft(TFT_MOSI, TFT_SCLK, TFT_CS, TFT_DC, TFT_RST, TFT_WIDTH, TFT_HEIGHT);
// //SD_Manager sd(TFT_SCLK, SD_MISO, TFT_MOSI, SD_CS, SD_BTN);
// // ---------------- HARDWARE ----------------
// Adafruit_MCP23X17 mcp;
// KeypadManager keypad(&mcp, rowPins, colPins, keys);
// NFCManager nfc(-1,-1);
// BatteryManager battery(MAX17048_ADDR, 3.3, 4.16);
// LEDManager leds(&mcp, ledPins, 8);
// Protocol protocol(mqttClient);
// // Buttons
// Button btnDown(BTN1);
// Button btnUp(BTN2);
// Button btnRight(BTN4);
// Button btnOK(BTN5);
// Button btnLeft(BTN3);
// // ---------------- FLAGS ----------------
// volatile bool wifiConnected = false;
// bool apModeActive = false;
// bool isBootPhase = true;
// // Add these with your other global variables
// String pending_auction_id = "";
// String current_nfc_uid = "";
// String current_user_name = "";
// bool nfc_validation_pending = false;
// unsigned long last_nfc_check = 0;

// // ---------------- TIMERS ----------------
// unsigned long now = 0;
// unsigned long lastKeypad = 0;
// unsigned long lastNFC = 0;
// unsigned long lastBattery = 0;
// unsigned long lastLED = 0;
// unsigned long lastWiFiCheck = 0;
// unsigned long lastTimeUpdate = 0;

// // ---------------- INTERVALS ----------------
// const unsigned long KEYPAD_INTERVAL = 20;
// const unsigned long NFC_INTERVAL = 150;
// const unsigned long BATTERY_INTERVAL = 5000;
// const unsigned long LED_INTERVAL = 80;
// const unsigned long WIFI_INTERVAL = 5000;
// const unsigned long TIME_INTERVAL = 2000;
// static const unsigned long NFC_CHECK_INTERVAL = 3000;
// bool nfcActive = false;
// unsigned long lastNFCTime = 0;
// const unsigned long NFC_COOLDOWN = 1000;
// char lastKey = 0;
// unsigned long lastKeyTime = 0;
// const unsigned long keyDebounce = 80; // ms

// bool mqttConfigured = false;
// bool mqttConnected = false;
// bool timeInitialized = false;
// unsigned long lastMQTTReconnect = 0;
// unsigned long lastMQTT = 0;
// unsigned long lastSend = 0;

// // ---------------- I2C LOCK ----------------
// volatile bool i2cBusy = false;
// String lastNfcUid;
// String uidString;

// enum UIState {
//     UI_AP_MODE,
//     UI_AUCTION,
//     UI_NFC,
//     UI_LOADING_ITEMS,
//     UI_ITEMS,
//     UI_BID_WAIT_NFC
// };
// UIState currentUI = UI_AUCTION;

// static lv_obj_t* main_screen = nullptr;
// static bool auction_screen_initialized = false;
// //static lv_obj_t* items_screen = nullptr;
// extern lv_obj_t* content_area;
// extern lv_obj_t* items_content_area;
// static bool items_screen_initialized = false;
// bool readNFC(uint8_t *uid, uint8_t &length);
// void updateBattery();
// char readKeypad();
// void updateInputs();
// void setupAWSMQTT();
// void connectMQTT(); 
// void startNTP();
// void updateTime();
// void on_access_response(bool granted, String userId, String userName);
// bool i2cLock();
// void i2cUnlock();
// void setup_auction_screen();
// void on_auctions_received();
// void on_items_received(String auctionId);
// void setup_items_screen();
// void handleAuction();
// void handleNFC();
// void handleItems();

// bool i2cLock() {
//   if (i2cBusy) return false;
//   i2cBusy = true;
//   return true;
// }
// void i2cUnlock() {
//   i2cBusy = false;
// }
// void mqttCallback(char* topic, byte* payload, unsigned int length) {
//     protocol.onMessage(topic, payload, length);
// }
// void setup_auction_screen() {
//     if (!auction_screen_initialized) {
//         main_screen = lv_scr_act();
//         //create_topbar(240, 320);
//         init_auction_screen(main_screen);
//         auction_screen_initialized = true;
//         Serial.println("Auction screen initialized");
//         if (protocol.getTotalAuctions() > 0) {
//             load_auctions_from_cache();
//             hide_status_message();
//             Serial.println("Auctions displayed");
//         }
//     }
// }
// void on_auctions_received() {
//     Serial.println("Auctions received!");
//     if (auction_screen_initialized) {
//         load_auctions_from_cache();
//         show_auction_loading(false);
//         hide_status_message();
//         Serial.print("Displayed ");
//         Serial.print(protocol.getTotalAuctions());
//         Serial.println(" auctions");
//     }
// }
// void on_items_received(String auctionId) {
//     Serial.println("Items received for auction: " + auctionId);
//     if (items_screen_initialized) {
//         load_items_for_auction(auctionId);    
//         // Hide auction screen and show items screen
//         hide_auction_screen();  // Use this function instead of lv_obj_add_flag
//         show_items_screen();
//         currentUI = UI_ITEMS;
//         hide_status_message();
//         Serial.println("Switched to items screen");
//     }
// }
// // Initialize items screen
// void setup_items_screen() {
//     if (!items_screen_initialized) {
//         main_screen = lv_scr_act();
//         init_items_screen(main_screen);
//         hide_items_screen(); // Start hidden
//         items_screen_initialized = true;
//         Serial.println("Items screen initialized");
//     }
// }
// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("SYSTEM START");
//   pinMode(POWER_PIN, OUTPUT);    //NFC Reader power pin
//   digitalWrite(POWER_PIN, HIGH);
//   Wire.begin(SDA_PIN, SCL_PIN);
//   pinMode(BTN5, INPUT);   // IMPORTANT FIX
//   Serial.println("Buttons initialized (INPUT)");
//   tft.begin();
//   delay(100);
//   tft.fillScreen(0xffff);
//   lvgl_init();
//   // Show logo
//   uint16_t x = (240 - 240) / 2;
//   uint16_t y = (320 - 80) / 2;
//   tft.drawImage(x, y, LOGO_WIDTH, LOGO_HEIGHT, myImage);
//   delay(3000);
//   create_topbar(240, 320);
//   show_status_message("Connecting to WiFi...", "\uF1EB");
//   if (!mcp.begin_I2C(0x20)) {
//     Serial.println("❌ MCP not found!");
//   } else {
//     Serial.println("✅ MCP OK");
//   }
//   protocol.begin("Device_001", AUCTION_REQ_TOPIC, AUCTION_RES_TOPIC);
//   protocol.setAuctionsCallback(on_auctions_received);
//   protocol.setItemsCallback(on_items_received);
//   protocol.setAccessCallback(on_access_response);
//   keypad.begin();
//   nfc.begin();
//   battery.begin();
//   leds.begin();
//   btnOK.begin();
//   btnUp.begin();
//   btnDown.begin();
//   btnLeft.begin();
//   btnRight.begin();

//   // ---------------- AP MODE CHECK ----------------
//   Serial.println("\nChecking AP mode trigger...");
//   unsigned long start = millis();
//   while (millis() - start < 2000) {
//       if (digitalRead(BTN5) == LOW) {
//           apModeActive = true;
//           Serial.println("AP mode activated by button press");
//           break;
//       }
//       delay(10);
//   }
//     // Wi-Fi in normal mode
//   if (!apModeActive) {
//       preferences.begin("wifi-config", true);
//       String ssid = preferences.getString("ssid", "");
//       String pass = preferences.getString("password", "");
//       preferences.end();
//       if (ssid.length() > 0) {
//           set_wifi_connected(false);
//           set_mqtt_connected(false);
//           WiFi.begin(ssid.c_str(), pass.c_str());
//           Serial.println("Connecting to WiFi: " + ssid);
//       }
//   }else{
//       apMode.begin();
//   }
//    hide_status_message();
//    setup_auction_screen();
//    setup_items_screen();
// }

// void loop() {
//   lv_timer_handler();
//   now = millis();
//   // ---------------- AP MODE ----------------
//   if (apModeActive) {
//     show_status_message("Ap Mode Activate", "\uF021");
//     currentUI = UI_AP_MODE;

//   }
//   if (WiFi.status() == WL_CONNECTED) {
//     if (!wifiConnected) {
//         wifiConnected = true;
//         set_wifi_connected(true);
//         set_mqtt_connected(false);
//         Serial.println("✅ WiFi Connected!");
//         lv_timer_handler();
//      ///show_status_message("WiFi Connected", "\uF1EB");
//         startNTP();
//     }
//     if (!timeInitialized) return;
//       setupAWSMQTT();
//     if (!mqttClient.connected()) {
//       connectMQTT();
//     } else {
//         mqttClient.loop();
//     }
//   }
//   // -------- KEYPAD --------
//   char key = readKeypad();
//   if (key != '\0') {
//       Serial.print("KEY: ");
//       Serial.println(key);
//   }
//   switch (currentUI) {
//     case UI_AP_MODE:
//         Serial.println("AP MODE RUNNING...");
//         show_status_message("Ap Mode Active", "\uF079");
//         apMode.handle();
//         delay(1000);
//         return;
//             break;

//     case UI_AUCTION:
//         hide_status_message();
//         handleAuction();
//         break;

//     case UI_NFC:
//         hide_status_message();
//         handleNFC();
//         break;
        
//     case UI_ITEMS:
//         hide_status_message();
//         break;

//     }
//   updateInputs(); 
//   updateBattery();
//   updateTime();
    
// }

// void handleAuction(){
//     // Only handle button presses when in auction mode
//     if (btnDown.pressed()) {
//         auction_next();
//     }
//     if (btnUp.pressed()) {
//         auction_previous();
//     }
//     if (btnOK.pressed()) {
//         // ✅ Only proceed if OK button is actually pressed
//         Serial.println("OK button pressed - preparing for NFC validation");
//         const char* auctionId = get_current_auction_id();
//         if (auctionId != nullptr) {
//             pending_auction_id = String(auctionId);
//             Serial.print("Selected auction: ");
//             Serial.println(pending_auction_id);
            
//             // Clear previous NFC data
//             current_nfc_uid = "";
//             current_user_name = "";
//             nfc_validation_pending = true;
//             last_nfc_check = millis();
//             // Switch to NFC validation state
//             currentUI = UI_NFC;
//             show_status_message("Please tap NFC card", "\uF0F6");
//         }
//     }
// }

// void handleNFC(){
//     if (millis() - last_nfc_check >= NFC_CHECK_INTERVAL) {
//         last_nfc_check = millis();
        
//         uint8_t uid[7];
//         uint8_t length;
//         if (readNFC(uid, length)) {
//             // Convert UID to hex string
//             String uidString = "";
//             for (int i = 0; i < length; i++) {
//                 if (uid[i] < 0x10) uidString += "0";
//                 uidString += String(uid[i], HEX);
//             }
//             uidString.toUpperCase();                    
//             Serial.print("NFC Tag Detected: ");
//             Serial.println(uidString);                   
//             // Send access check
//             show_status_message("Validating NFC...", "\uF0F6");
//             protocol.sendCheckAccess(uidString);
//             // Stay in validation state until callback response
//         } else {
//             // No NFC detected, continue waiting
//             static int check_counter = 0;
//             check_counter++;
//             if (check_counter % 2 == 0) { // Every 6 seconds
//                 Serial.println("Waiting for NFC card...");
//                 show_status_message("Please tap NFC card", "\uF0F6");
//             }
//         }
//     }
// }

// void Handleitems(){
//     if (btnDown.pressed()) {
//         item_next();
//     }
//     if (btnUp.pressed()) {
//         item_previous();
//     }
//     if (btnLeft.pressed()) {
//         hide_items_screen();
//         show_auction_screen();  // Use this function instead
//         currentUI = UI_AUCTION;
//         Serial.println("Returned to auction screen");
//     }
// }

// char readKeypad() {
//     if (millis() - lastKeypad < KEYPAD_INTERVAL) return '\0';
//     lastKeypad = millis();
//     if (!i2cLock()) return '\0';
//     char key = keypad.scan();
//     i2cUnlock();
//     if (key == '\0') {
//         lastKey = '\0';
//         return '\0';
//     }
//     if (key != lastKey) {
//         lastKey = key;
//         lastKeyTime = millis();
//         return '\0';
//     }
//     if ((millis() - lastKeyTime) > keyDebounce) {
//         char detectedKey = key;
//         lastKey = '\0'; // prevent repeat
//         return detectedKey;
//     }
//     return '\0';
// }
// void updateInputs() {
//     btnDown.update();
//     btnUp.update();
//     btnRight.update();
//     btnLeft.update();
//     btnOK.update();
// }
// void updateBattery() {
//     if (millis() - lastBattery < BATTERY_INTERVAL) return;
//     lastBattery = millis();
//     if (!i2cLock()) return;
//     uint8_t pct = (uint8_t)battery.readPercent();
//     set_battery_percent(pct);
//     i2cUnlock();
//     Serial.print("Battery: "); Serial.print(pct); Serial.println("%");
// }
// void updateTime() {
//     if (millis() - lastTimeUpdate < TIME_INTERVAL) return;
//     lastTimeUpdate = millis();

//     struct tm timeinfo;
//     if (getLocalTime(&timeinfo)) {
//         char buf[20];
//         snprintf(buf, sizeof(buf), "%02d:%02d %02d/%02d",
//                  timeinfo.tm_hour, timeinfo.tm_min,
//                  timeinfo.tm_mday, timeinfo.tm_mon + 1);
//         set_date_time(buf);
//     }
// }

// bool readNFC(uint8_t *uid, uint8_t &length) {
//     if (millis() - lastNFC < NFC_INTERVAL) return false;
//     lastNFC = millis();
//     if (!i2cLock()) return false;
//     uint8_t tempUID[7];
//     uint8_t tempLength;
//     bool found = nfc.readUID(tempUID, &tempLength);
//     i2cUnlock();
//     if (found) {
//         if (nfcActive && (millis() - lastNFCTime < NFC_COOLDOWN)) {
//             return false;
//         }
//         nfcActive = true;
//         lastNFCTime = millis();
//         length = tempLength;
//         for (int i = 0; i < tempLength; i++) {
//             uid[i] = tempUID[i];
//         }
//         return true;
//     } 
//     else {
//         nfcActive = false;
//     }
//     return false;
// }
// void setupAWSMQTT() {

//     if (mqttConfigured) return;

//     net.setCACert(AWS_ROOT_CA);
//     net.setCertificate(AWS_CERT_CRT);
//     net.setPrivateKey(AWS_PRIVATE_KEY);

//     net.setTimeout(15000);
//     net.setHandshakeTimeout(15000);

//     mqttClient.setServer(awsEndpoint, 8883);
//     mqttClient.setCallback(mqttCallback);
//     mqttClient.setBufferSize(4096);

//     mqttConfigured = true;

//     Serial.println("AWS MQTT configured");
//    //show_status_message("Backend configured", "\uF074");
// }

// void connectMQTT() {

//     if (mqttClient.connected()) return;

//     if (millis() - lastMQTTReconnect < 3000) return;
//     lastMQTTReconnect = millis();

//     Serial.print("Connecting AWS IoT...");
//     set_mqtt_connected(false);

//     String clientId = "device-" + String((uint32_t)ESP.getEfuseMac(), HEX);

//     if (mqttClient.connect(clientId.c_str())) {

//         Serial.println("CONNECTED!");
//         mqttClient.subscribe(AUCTION_RES_TOPIC);
//         mqttConnected = true;
//         set_mqtt_connected(true);
//         delay(1000);
//         protocol.sendGetAuction();

//     } else {
//         Serial.print("FAILED rc=");
//         Serial.println(mqttClient.state());
//         mqttConnected = false;
//         set_mqtt_connected(false);
//     }
// }
// void startNTP() {
//     configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
// //  show_status_message("Syncing Time...", "\uF021");
//     Serial.println("NTP started - Syncing time...");
    
//     // Wait for time to sync
//     struct tm timeinfo;
//     int attempts = 0;
//     while (!getLocalTime(&timeinfo, 5000) && attempts < 10) {
//         Serial.print(".");
//         delay(1000);
//         attempts++;
//     }
    
//     if (attempts < 10) {
//         Serial.println("\n✓ Time synchronized!");
//         char buf[30];
//         strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
//         Serial.print("Current time: ");
//         Serial.println(buf);
// //      show_status_message("Time Synced", "\uF021");
//         timeInitialized = true;
//     } else {
//         Serial.println("\n✗ Failed to sync time!");
// //       show_status_message("Failed", "\uF021");
//         timeInitialized = false;
//     }
// }
// void on_access_response(bool granted, String userId, String userName) {
//     Serial.print("Access response - Granted: ");
//     Serial.println(granted ? "YES" : "NO");
    
//     if (granted) {
//         current_user_name = userName;
//         current_nfc_uid = protocol.getCurrentNfcUid();
        
//         Serial.print("User validated: ");
//         Serial.println(current_user_name);
        
//         // Show success message
//         show_status_message(("Welcome " + current_user_name).c_str(), "\uF007");
//         delay(1500);
        
//         // Move to items screen
//         hide_auction_screen();
//         show_items_screen();
//         currentUI = UI_ITEMS;
//         nfc_validation_pending = false;
//         hide_status_message();
        
//         // Load items for the selected auction
//         if (pending_auction_id.length() > 0) {
//             show_items_loading(true);
//             protocol.sendGetItems(pending_auction_id);
//         }
//     } else {
//         // Access denied
//         show_status_message("Access Denied! Invalid NFC", "\uF00D");
//         delay(2000);
        
//         // Clear validation state
//         current_user_name = "";
//         current_nfc_uid = "";
//         nfc_validation_pending = false;
//         pending_auction_id = "";
        
//         // Return to auction screen
//         currentUI = UI_AUCTION;
//         hide_status_message();
//     }
// }





#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <logo.h>
#include <PubSubClient.h>
#include <LVGLScreen.h>
#include <tft_init.h>
#include <Hardware.h>
#include <Preferences.h>
#include <apmode.h>
#include <Button.h>
#include <Secret.h>
#include <ArduinoJson.h>
#include <Protocol.h>
#include <Header_template.h>
#include <AuctionScreen.h>
#include <ItemsScreen.h>
// ---------------- I2C ----------------
#define SDA_PIN 16
#define SCL_PIN 17
#define POWER_PIN 12
#define MAX17048_ADDR 0x36

// ---------------- POWER & LATCH ----------------
#define POWER_LATCH_PIN 4
#define POWER_BTN_PIN   14

// ---------------- LCD POWER ----------------
#define LCD_SW_PIN  2   // GPIO2 -> Q9 MOSFET -> enables display GND

// ---------------- TFT ----------------
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5
#define TFT_DC   21
#define TFT_RST  22

#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define LOGO_WIDTH 240
#define LOGO_HEIGHT 80


// ---------------- BUTTONS ----------------
#define BTN1 26
#define BTN2 27
#define BTN3 25
#define BTN4 32
#define BTN5 33

// ------------------ MQTT ------------------
const char* AUCTION_REQ_TOPIC = "auction/Device_001/request";
const char* AUCTION_RES_TOPIC = "auction/Device_001/response";
const char* awsEndpoint = "a1m322vfibs32e-ats.iot.ap-south-1.amazonaws.com";
String selectedAuctionId; // For testing, this will be set when user selects an auction

// ------------------ GLOBALS ------------------
String deviceId = "Device_001";
String clientId;
String firmwareVersion = "1.0.0";
String hardwareVersion = "1.0";

// ---------------- SYSTEM ----------------
WiFiClientSecure net;
PubSubClient mqttClient(net);
APMode apMode("AuctionHub", "12345678");
Preferences preferences;

// ---------------- KEYPAD/LED ----------------
const uint8_t rowPins[5] = {0,1,2,3,4};
const uint8_t colPins[3] = {5,6,7};
const uint8_t ledPins[] = {8,9,10,11,12,13,14,15};
char keys[5][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
  {'A','B','C'}
};

TFT_init tft(TFT_MOSI, TFT_SCLK, TFT_CS, TFT_DC, TFT_RST, TFT_WIDTH, TFT_HEIGHT);
// //SD_Manager sd(TFT_SCLK, SD_MISO, TFT_MOSI, SD_CS, SD_BTN);
// ---------------- NFC ----------------
const uint8_t nfcIRQ = -1;
const uint8_t nfcReset = -1;

// ---------------- GLOBALS ----------------
Adafruit_MCP23X17 mcp;
KeypadManager keypad(&mcp, rowPins, colPins, keys);
LEDManager leds(&mcp, ledPins, 8);
NFCManager nfc(nfcIRQ, nfcReset);
BatteryManager battery(MAX17048_ADDR, 3.3, 4.16);
Protocol protocol(mqttClient);

Button btnDown(BTN1);
Button btnUp(BTN2);
Button btnLeft(BTN3);
Button btnRight(BTN4);
Button btnOK(BTN5);
Button btnPower(POWER_BTN_PIN);

volatile bool i2cBusy = false;
bool wifiConnected = false;
bool mqttConnected = false;
bool nfcActive = false;
bool apModeActive = false;

unsigned long lastKeypad = 0;
unsigned long lastNFC = 0;
unsigned long lastBattery = 0;
unsigned long lastNFCTime = 0;
char lastKey = 0;

const unsigned long KEYPAD_INTERVAL = 20;
const unsigned long NFC_INTERVAL = 150;
const unsigned long BATTERY_INTERVAL = 5000;
const unsigned long NFC_COOLDOWN = 1000;
const unsigned long keyDebounce = 80;
unsigned long lastKeyTime = 0; 

unsigned long lastWiFiCheck = 0;
const unsigned long WIFI_INTERVAL = 5000; // check every 5 sec
unsigned long lastMQTTReconnect = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length);
void updateBattery();
void updateTime();
void updateInputs();
char readKeypad();  
bool readNFC(uint8_t *uid, uint8_t &length);
void checkMQTT();
void connectMQTT();

// ---------------- I2C LOCK ----------------
bool i2cLock() {
    if (i2cBusy) return false;
    i2cBusy = true;
    return true;
}
void i2cUnlock() { i2cBusy = false; }


// ---------------- SETUP ----------------
void setup() {
    // ---------------- LATCH POWER ON ----------------
    pinMode(POWER_LATCH_PIN, OUTPUT);
    digitalWrite(POWER_LATCH_PIN, HIGH);

    Serial.begin(115200);
    
    // Enable display ground (CRITICAL - Q9 MOSFET on GPIO2)
    pinMode(LCD_SW_PIN, OUTPUT);
    digitalWrite(LCD_SW_PIN, HIGH);
    Serial.println("[DEBUG] LCD ground enabled (GPIO2 HIGH)");
    delay(50);  // Let display power stabilize
    
    Wire.begin(SDA_PIN, SCL_PIN);
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);
    pinMode(BTN5, INPUT_PULLUP);   // IMPORTANT FIX: Prevent floating pin from triggering AP mode
    Serial.println("Buttons initialized (INPUT)");
    Serial.println("[DEBUG] Starting TFT init...");
    
    // Disable SD Card SPI interference (CS = GPIO 13)
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(10);

    tft.begin();
    Serial.println("[DEBUG] TFT init done. Filling screen RED...");
    delay(100);
    tft.fillScreen(0xF800);  // RED for diagnostic - should be clearly visible!
    Serial.println("[DEBUG] Fill screen done. Waiting 3s...");
    delay(3000);  // Wait 3 seconds to see if RED appears
    Serial.println("[DEBUG] Init LVGL...");
    lvgl_init();
    Serial.println("[DEBUG] Drawing logo...");
    // Show logo
    uint16_t x = (240 - 240) / 2;
    uint16_t y = (320 - 80) / 2;
    tft.drawImage(x, y, LOGO_WIDTH, LOGO_HEIGHT, myImage);
    Serial.println("[DEBUG] Logo drawn. Waiting 3s...");
    delay(3000);
    Serial.println("[DEBUG] Creating topbar...");
    create_topbar(240, 320);
    // MCP23X17
    if (!mcp.begin_I2C(0x20)) {
        Serial.println("❌ MCP not found!");
        while(1);
    }
    // ---------------- AP MODE CHECK ----------------
    Serial.println("\nChecking AP mode trigger...");
    unsigned long start = millis();
    while (millis() - start < 2000) {
        if (digitalRead(BTN5) == LOW) {
            apModeActive = true;
            Serial.println("AP mode activated by button press");
            break;
        }
        delay(10);
    }
        // Wi-Fi in normal mode
    if (!apModeActive) {
        // Hardcoded Wi-Fi credentials
        String ssid = "SLT-Fiber-2.4G_2880";
        String pass = "coin6657";
        if (ssid.length() > 0) {
            set_wifi_connected(false);
            set_mqtt_connected(false);
            WiFi.begin(ssid.c_str(), pass.c_str());
            Serial.print("Connecting to WiFi: ");
            Serial.println(ssid);
            
            // Wait for connection
            int timeout = 20; // 10 seconds (20 * 500ms)
            while (WiFi.status() != WL_CONNECTED && timeout > 0) {
                delay(500);
                Serial.print(".");
                timeout--;
            }
            Serial.println();
            
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("WiFi Connected!");
                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());
                set_wifi_connected(true);
                
                // Initialize NTP Time (Sri Lanka +05:30)
                configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
                Serial.println("Waiting for NTP time sync...");
            } else {
                Serial.println("WiFi Connection Failed!");
            }
        }
    }else{
        apMode.begin();
    }
    protocol.begin("Device_001", AUCTION_REQ_TOPIC, AUCTION_RES_TOPIC);
    //protocol.setAuctionsCallback(on_auctions_received);
    //protocol.setItemsCallback(on_items_received);
    //protocol.setAccessCallback(on_access_response);
    // Initialize hardware
    keypad.begin();
    leds.begin();
    nfc.begin();
    battery.begin();

    // Buttons
    btnDown.begin();
    btnUp.begin();
    btnLeft.begin();
    btnRight.begin();
    btnOK.begin();
    btnPower.begin();

    Serial.println("Hardware initialized");
}

void checkPowerOff() {
    if (btnPower.pressed()) {
        Serial.println("Power OFF requested! Please release the button...");
        
        // Wait until the power button is released
        // Assuming LOW means pressed, we wait until it goes HIGH
        while (digitalRead(POWER_BTN_PIN) == LOW) {
            delay(10);
        }
        delay(100); // Small debounce delay after release
        
        Serial.println("Button released. Setting Latch Pin 4 LOW...");
        Serial.flush(); // Ensure the message prints before power dies
        
        digitalWrite(POWER_LATCH_PIN, LOW);
        while (1) {
            delay(10);
        }
    }
}

// ---------------- LOOP ----------------
void loop() {
    lv_timer_handler(); // Required for LVGL to update the display!
    
    updateInputs();
    checkPowerOff();
    updateBattery();
    updateTime();

    // ---------------- Keypad ----------------
    char key = readKeypad();
    if (key != '\0') {
        Serial.print("Key pressed: ");
        Serial.println(key);
    }

    // ---------------- NFC ----------------
    uint8_t uid[7];
    uint8_t uidLength;
    if (readNFC(uid, uidLength)) {     // now returns bool
        Serial.print("NFC UID: ");
        for (uint8_t i=0; i<uidLength; i++) {
            Serial.print(uid[i], HEX); Serial.print(" ");
        }
        Serial.println();
    }
}

// ---------------- INPUTS ----------------
void updateInputs() {
    btnDown.update();
    btnUp.update();
    btnLeft.update();
    btnRight.update();
    btnOK.update();
    btnPower.update();
}

// ---------------- BATTERY ----------------
void updateBattery() {
    if (millis() - lastBattery < BATTERY_INTERVAL) return;
    lastBattery = millis();
    if (!i2cLock()) return;

    uint8_t pct = (uint8_t)battery.readPercent();
    Serial.print("Battery: "); Serial.print(pct); Serial.println("%");
    set_battery_percent(pct); // Update LVGL screen

    i2cUnlock();
}

// ---------------- TIME ----------------
unsigned long lastTimeUpdate = 0;
void updateTime() {
    if (millis() - lastTimeUpdate >= 60000 || lastTimeUpdate == 0) {
        lastTimeUpdate = millis() == 0 ? 1 : millis();
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            char timeStringBuff[50];
            strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M %d/%m", &timeinfo);
            set_date_time(timeStringBuff);
        }
    }
}

// ---------------- KEYPAD ----------------
char readKeypad() {
    if (millis() - lastKeypad < KEYPAD_INTERVAL) return '\0';
    lastKeypad = millis();
    if (!i2cLock()) return '\0';
    char key = keypad.scan();
    i2cUnlock();

    if (key == '\0') {
        lastKey = '\0';
        return '\0';
    }
    if (key != lastKey) {
        lastKey = key;
        lastKeyTime = millis();
        return '\0';
    }
    if ((millis() - lastKeyTime) > keyDebounce) {
        lastKey = '\0';
        return key;
    }
    return '\0';
}

// ---------------- NFC ----------------
bool readNFC(uint8_t *uid, uint8_t &length) {
    if (millis() - lastNFC < NFC_INTERVAL) return false;
    lastNFC = millis();
    if (!i2cLock()) return false;
    uint8_t tempUID[7];
    uint8_t tempLength;
    bool found = nfc.readUID(tempUID, &tempLength);
    i2cUnlock();
    if (found) {
        if (nfcActive && (millis() - lastNFCTime < NFC_COOLDOWN)) return false;
        nfcActive = true;
        lastNFCTime = millis();
        length = tempLength;
        for (uint8_t i=0; i<tempLength; i++) uid[i] = tempUID[i];
        return true;
    } else {
        nfcActive = false;
    }

    return false;
}
void checkWiFi() {
    if (millis() - lastWiFiCheck < WIFI_INTERVAL) return;
    lastWiFiCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {
        if (!wifiConnected) {
            wifiConnected = true;
            Serial.println("✅ WiFi Connected");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
        }
    } else {
        if (wifiConnected) {
            Serial.println("❌ WiFi Lost!");
            wifiConnected = false;
        }

        Serial.println("Reconnecting WiFi...");
        WiFi.reconnect();
    }
}
void checkMQTT() {
    if (!wifiConnected) return;

    if (!mqttClient.connected()) {
        Serial.println("Reconnecting MQTT...");
        connectMQTT();
    } else {
        mqttClient.loop();
    }
}
void connectMQTT() {

    if (mqttClient.connected()) return;

    if (millis() - lastMQTTReconnect < 3000) return;
    lastMQTTReconnect = millis();

    Serial.print("Connecting AWS IoT...");
    set_mqtt_connected(false);

    String clientId = "device-" + String((uint32_t)ESP.getEfuseMac(), HEX);

    if (mqttClient.connect(clientId.c_str())) {

        Serial.println("CONNECTED!");
        mqttClient.subscribe(AUCTION_RES_TOPIC);
        mqttConnected = true;
        set_mqtt_connected(true);
        delay(1000);
        protocol.sendGetAuction();

    } else {
        Serial.print("FAILED rc=");
        Serial.println(mqttClient.state());
        mqttConnected = false;
        set_mqtt_connected(false);
    }
}





























  // if (btnOK.pressed()) {
  //     Serial.println("Button OK Pressed - Sending MQTT Message");
  //     protocol.sendGetAuction();
  // }
  // if (btnLeft.pressed()) {
  //     Serial.println("Button Left Pressed - Requesting Items");
  //     if (selectedAuctionId.length() > 0) {
  //         protocol.sendGetItems(selectedAuctionId);
  //     } else {
  //         Serial.println("No auction selected!");
  //     }
  // }
  // if (btnUp.pressed()) {
  //   Serial.println("Button Up Pressed - Submitting Bid");
    
  //   // Get the stored NFC UID from when you scanned the card
  //   if (uidString.length() > 0) {
  //       // Example bid data
  //       String auctionId = "AUC001";      // Get from selected auction
  //       String itemId = "ITEM001";         // Get from selected item
  //       float bidAmount = 550.00;          // Get from keypad input
  //       String currency = "LKR";
        
  //       // Send the bid
  //       protocol.sendSubmitBid(auctionId, itemId, uidString, bidAmount, currency);
  //   } else {
  //       Serial.println("No NFC card scanned! Please scan your card first.");
  //   }
  // }
  // if (btnRight.pressed()) {
  //   Serial.println("Button Right Pressed - Sending MQTT Message");
    
  //   uint8_t uid[7];
  //   uint8_t length;
    
  //   if (readNFC(uid, length)) {
  //       // Convert UID to hex string
  //       for (int i = 0; i < length; i++) {
  //           if (uid[i] < 0x10) uidString += "0";
  //           uidString += String(uid[i], HEX);
  //       }
  //       uidString.toUpperCase();
        
  //       Serial.print("NFC UID: ");
  //       Serial.println(uidString);
        
  //       // Send access check with the UID we just read
  //       protocol.sendCheckAccess(uidString);
  //       uidString = ""; // Clear after use
  //   } else {
  //       Serial.println("No NFC card detected! Please scan a card first.");
  //   }
  // }