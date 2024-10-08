// external library
#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include <Keypad.h>
#include <LittleFS.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Wire.h>
#include "wsEventHandler/wsEventHandler.h"

// custom library
#include "first_page/first_page.h"
#include "second_page/second_page.h"
#include "third_page/third_page.h"
#include "fourth_page/fourth_page.h"
#include "fifth_page/fifth_page.h"
#include "sixth_page/sixth_page.h"

// Definitions
#define FS LittleFS
#define WIFI_SSID "abass"
#define WIFI_PASSWORD "abcdefghij"
#define EEPROM_SIZE 512
#define RX_FROM_ARDUINO_NANO 23
#define TX_FROM_ARDUINO_NANO 22
#define RX_FROM_ATMEGA 21 // Custom RX pin (connected to ATmega TX)
#define TX_FROM_ATMEGA 19 // Custom TX pin (connected to ATmega RX)
#define BUFFER_SIZE 512   // Allocate memory for sending JSON data
#define BUZZER_PIN 18

enum SELECTED_TEST
{
  TENSION = 1,
  COMPRESSION = 2,
  TORSION = 3
};

enum CURRENT_PAGE
{
  FIRST = 1,
  SECOND = 2,
  THIRD = 3,
  FOURTH = 4,
  FIFTH = 5,
  FIFTH_ONE = 51,
  FIFTH_TWO = 52,
  SIXTH = 6,
  SIXTH_ONE = 61,
  SEVENTH = 7
};

// Address
int SELECT_TEST_VALUE_ADDRESS = 0;
int SELECTED_PAGE_ADDRESS = 5;
int LOADCELL_RATING_ADDRESS = 15;
int SENSOR_RATING_ADDRESS = 50;
int MAX_FORCE_ADDRESS = 100;
int EMPTY_ADDRESS = 150;
int TARGET_EXTENSION_ADDRESS = 250;

int SELECTED_TEST_VALUE = 0;
int SELECTED_PAGE = FIRST;
String input_value = "";
String SENSOR_RATING_IN_KG = "-1";
String TARGET_FORCE = "-1";
String TARGET_EXTENSION = "-1";
int EMPTY;

// Variable declarations/instantations
const byte ROWS = 4;
const byte COLS = 3;
byte rowPins[ROWS] = {33, 25, 26, 27};
byte colPins[COLS] = {14, 12, 13};
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

// Object instantiations
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/5, /* CS=*/16, /* reset=*/17);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

unsigned long lastSentTime = 0;
const unsigned long messageInterval = 1000; // Send every 100 ms

void sendWebSocketMessage(String test_type, String sensor_rating, String target_force, String target_extension)
{
  if (millis() - lastSentTime >= messageInterval)
  {

    broadcastSixthPageInfo(test_type, sensor_rating, target_force, target_extension);

    lastSentTime = millis();
  }
}
String selectedTest()
{
  if (SELECTED_TEST_VALUE == TENSION)
  {
    return "tension";
  }
  else if (SELECTED_TEST_VALUE == COMPRESSION)
  {
    return "compression";
  }
  else if (SELECTED_TEST_VALUE == TORSION)
  {
    return "torsion";
  }
}
void setup()
{
  Serial.begin(115200);
  Serial1.begin(4800, SERIAL_8N1, RX_FROM_ATMEGA, TX_FROM_ATMEGA);
  Serial2.begin(4800, SERIAL_8N1, RX_FROM_ARDUINO_NANO, TX_FROM_ARDUINO_NANO);

  pinMode(BUZZER_PIN, OUTPUT);

  u8g2.begin();
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("Failed to initialize EEPROM");
    return;
  }

  if (!FS.begin())
  {
    Serial.println("Failed to mount LittleFS. Formatting...");
    // Force format
    if (FS.format())
    {
      Serial.println("LittleFS formatted successfully.");
      // Try to mount again after formatting
      if (!FS.begin())
      {
        Serial.println("Failed to mount LittleFS after formatting.");
      }
    }
    else
    {
      Serial.println("Failed to format LittleFS.");
    }
  }
  else
  {
    Serial.println("LittleFS mounted successfully.");
  }
  // bind websocket to async web server
  websocket.onEvent(wsEventHandler);
  server.addHandler(&websocket);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /index.html");
              AsyncWebServerResponse *response = request->beginResponse(FS, "/index.html", "text/html");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /main.js");
              AsyncWebServerResponse *response = request->beginResponse(FS, "/main.js", "text/javascript");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/digital.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Serving file:  /digital.ttf");
    AsyncWebServerResponse *response = request->beginResponse(FS, "/digital.ttf", "font/ttf");
    response->addHeader("Content-Encoding", "gzip"); 
    request->send(response); });

  server.on("/light.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Serving file:  /light.ttf");
    AsyncWebServerResponse *response = request->beginResponse(FS, "/light.ttf", "font/ttf");
    response->addHeader("Content-Encoding", "gzip"); 
    request->send(response); });

  server.on("/regular.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Serving file:  /regular.ttf");
    AsyncWebServerResponse *response = request->beginResponse(FS, "/regular.ttf", "font/ttf");
    response->addHeader("Content-Encoding", "gzip"); 
    request->send(response); });

  server.on("/semibold.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Serving file:  /semibold.ttf");
    AsyncWebServerResponse *response = request->beginResponse(FS, "/semibold.ttf", "font/ttf");
    response->addHeader("Content-Encoding", "gzip"); 
    request->send(response); });

  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /main.css");
              AsyncWebServerResponse *response = request->beginResponse(FS, "/main.css", "text/css");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/payload", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      DynamicJsonDocument jsonDoc(BUFFER_SIZE); // Use DynamicJsonDocument
      // Populate the JSON document with data
      String test;
    if (SELECTED_PAGE == SIXTH)
    {

      // Create a JSON document (either Dynamic or Static)
     
    jsonDoc["test_type"] = selectedTest();
    jsonDoc["sensor_rating"] = SENSOR_RATING_IN_KG;
    jsonDoc["target_force"] = TARGET_FORCE; // You can add any data you want to send
    }else {
       jsonDoc["test_type"] = "";
    jsonDoc["sensor_rating"] = "";
    jsonDoc["target_force"] = ""; 
    jsonDoc["target_extension"] = ""; 
    }

    // Serialize the JSON document to a string
    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);

    // Send JSON response back to the client
    request->send(200, "application/json", jsonResponse); });

  // Print the amount of free heap memory

  EEPROM.get(SELECT_TEST_VALUE_ADDRESS, SELECTED_TEST_VALUE);

  first_page_ui(u8g2);
  second_page_ui(u8g2);
  third_page_ui(u8g2, WIFI_SSID, WIFI_PASSWORD);
  server.begin();
  Serial.println("Server Started");
  fourth_page_ui(u8g2, SELECTED_PAGE_ADDRESS, FOURTH);
}

void loop()
{
  String test;
  char key = keypad.getKey();
  if (SELECTED_PAGE == FIFTH)
  {
    if (SENSOR_RATING_IN_KG == "-1")
    {

      EEPROM.get(SENSOR_RATING_ADDRESS, SENSOR_RATING_IN_KG);
      input_value = SENSOR_RATING_IN_KG;
    }

    fifth_page_ui(u8g2, SELECTED_PAGE, SELECTED_PAGE_ADDRESS, key, selectedTest(), SENSOR_RATING_ADDRESS, input_value, SENSOR_RATING_IN_KG, TARGET_FORCE, SELECTED_PAGE, TARGET_EXTENSION, -1, "");
  }
  if (SELECTED_PAGE == FIFTH_ONE)
  {
    if (TARGET_FORCE == "-1")
    {
      EEPROM.get(MAX_FORCE_ADDRESS, TARGET_FORCE);

      input_value = TARGET_FORCE;
    }
    fifth_page_ui(u8g2, SELECTED_PAGE, SELECTED_PAGE_ADDRESS, key, selectedTest(), MAX_FORCE_ADDRESS, input_value, SENSOR_RATING_IN_KG, TARGET_FORCE, SELECTED_PAGE, TARGET_EXTENSION, TARGET_EXTENSION_ADDRESS, "");
  }
  if (SELECTED_PAGE == FIFTH_TWO)
  {
    if (TARGET_EXTENSION == "-1")
    {
      EEPROM.get(TARGET_EXTENSION_ADDRESS, TARGET_EXTENSION);

      input_value = TARGET_EXTENSION;
    }
    fifth_page_ui(u8g2, SELECTED_PAGE, SELECTED_PAGE_ADDRESS, key, selectedTest(), TARGET_EXTENSION_ADDRESS, input_value, SENSOR_RATING_IN_KG, TARGET_FORCE, SELECTED_PAGE, TARGET_EXTENSION, -1, EXTENSOMETER_RATING_IN_MM);
  }

  if (SELECTED_PAGE == SIXTH)
  {

    if (SELECTED_TEST_VALUE == TENSION)
    {
      test = "tension";
    }
    else if (SELECTED_TEST_VALUE == COMPRESSION)
    {
      test = "compression";
    }
    else if (SELECTED_TEST_VALUE == TORSION)
    {
      test = "torsion";
    }
    sixth_page_ui(u8g2, key, test, SENSOR_RATING_IN_KG, TARGET_FORCE, TARGET_EXTENSION, SELECTED_PAGE_ADDRESS, SELECTED_PAGE);
  }

  if (SELECTED_PAGE == FOURTH)
  {
    fourth_page_ui(u8g2, SELECTED_PAGE_ADDRESS, FOURTH);
    EEPROM.get(SENSOR_RATING_ADDRESS, SENSOR_RATING_IN_KG);
    input_value = SENSOR_RATING_IN_KG;
  }
  sendWebSocketMessage(selectedTest(), SENSOR_RATING_IN_KG, TARGET_FORCE, TARGET_EXTENSION);
  if (key)
  {
    EEPROM.get(SELECTED_PAGE_ADDRESS, SELECTED_PAGE);

    // if()
    if (SELECTED_PAGE == FOURTH)
    {
      if (key - '0' == FIRST)
      {
        EEPROM.put(SELECT_TEST_VALUE_ADDRESS, TENSION);
        EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
      }
      else if (
          key - '0' == SECOND)
      {
        EEPROM.put(SELECT_TEST_VALUE_ADDRESS, COMPRESSION);
        EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
      }
      else if (key - '0' == THIRD)
      {
        EEPROM.put(SELECTED_TEST_VALUE, TORSION);
        EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
      }

      EEPROM.commit();
      EEPROM.get(SELECTED_PAGE_ADDRESS, SELECTED_PAGE);
      EEPROM.get(SELECT_TEST_VALUE_ADDRESS, SELECTED_TEST_VALUE);
    }
  }
}
