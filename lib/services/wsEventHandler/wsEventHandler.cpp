#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"
#include "types/status.h"

// allocate memory for recieved json data
#define BUFFER_SIZE 512
StaticJsonDocument<BUFFER_SIZE> recievedJson;
// initial device state
char dataBuffer[BUFFER_SIZE] = "{\"type\":\"message\",\"LED\":false,\"rating\":\"\"}";

AsyncWebSocketClient *clients[16];

// Example function to broadcast data to all connected clients
void broadcast_reading(double force, double displacement)
{
  StaticJsonDocument<BUFFER_SIZE> jsonToSend;
  // Prepare the data to be sent out
  jsonToSend["type"] = "update";
  jsonToSend["force_in_newton"] = abs(force);
  jsonToSend["displacement_in_mm"] = floor(abs(displacement)); 

  // Serialize the JSON data into the dataBuffer
  size_t len = serializeJson(jsonToSend, dataBuffer);

  // Send data to all connected clients
  for (int i = 0; i < 16; ++i)
  {
    if (clients[i] != NULL)
    {
      clients[i]->text(dataBuffer, len); // Broadcast the data
    }
  }
}

void broadcastPage(String page)
{

  StaticJsonDocument<BUFFER_SIZE> jsonToSend;
  jsonToSend["currentPage"] = page;
  jsonToSend["type"] = "current_page";

  // Serialize the JSON data into the dataBuffer
  size_t len = serializeJson(jsonToSend, dataBuffer);

  // Send data to all connected clients
  for (int i = 0; i < 16; ++i)
  {
    if (clients[i] != NULL)
    {
      clients[i]->text(dataBuffer, len); // Broadcast the data
    }
  }
}

void broadcastSixthPageInfo(String test_type, String sensor_rating, String target_force, String target_extension)
{

  StaticJsonDocument<BUFFER_SIZE> jsonToSend;
  jsonToSend["type"] = "dashboard_page_info";
  jsonToSend["test_type"] = test_type;
  jsonToSend["sensor_rating_kg"] = sensor_rating;
  jsonToSend["target_force"] = target_force;
  jsonToSend["target_extension"] = target_extension;
  jsonToSend["read_me_ish"] = "Multipy the sensor_rating_kg by 9.81";

  // Serialize the JSON data into the dataBuffer
  size_t len = serializeJson(jsonToSend, dataBuffer);

  // Send data to all connected client
  for (int i = 0; i < 16; ++i)
  {
    if (clients[i] != NULL)
    {
      clients[i]->text(dataBuffer, len); // Broadcast the data
    }
  }
}

void broadcastStatus(Status status)
{

  /*

   state can be any of the following :
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +                      NOT_STARTED  = 0                   +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +                       STARTED  = 1                      +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +                   NEEDS STOPPING  = 2                   +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +                      STOPPED   = 3                      +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  */
  String statusJson = "{";
  statusJson += "\"0\":\"test has not started\",";
  statusJson += "\"1\":\"test has started\",";
  statusJson += "\"2\":\"test needs stopping\",";
  statusJson += "\"3\":\"test has stopped\"";
  statusJson += "}";

  StaticJsonDocument<BUFFER_SIZE> jsonToSend;
  jsonToSend["type"] = "status";
  jsonToSend["status"] = status;
  jsonToSend["readme"] = statusJson;

  // Serialize the JSON data into the dataBuffer
  size_t len = serializeJson(jsonToSend, dataBuffer);

  // Send data to all connected client
  for (int i = 0; i < 16; ++i)
  {
    if (clients[i] != NULL)
    {
      clients[i]->text(dataBuffer, len); // Broadcast the data
    }
  }
}
void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_DATA)
  {
    // save the response as newest device state
    for (int i = 0; i < len; ++i)
      dataBuffer[i] = data[i];
    dataBuffer[len] = '\0';
#ifdef VERBOSE
    Serial.println(dataBuffer);
#endif
    // parse the recieved json data
    DeserializationError error = deserializeJson(recievedJson, (char *)data, len);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    if (strcmp(recievedJson["type"], "message") != 0)
      return;
    // get the target LED state
    bool led = recievedJson["LED"];
    // digitalWrite(LED_PIN, led);
    // send ACK
    client->text(dataBuffer, len);
    // alert all other clients
    for (int i = 0; i < 16; ++i)
      if (clients[i] != NULL && clients[i] != client)
        clients[i]->text(dataBuffer, len);
  }
  else if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    // ACK with current state
    client->text(dataBuffer);
    // store connected client
    for (int i = 0; i < 16; ++i)
      if (clients[i] == NULL)
      {
        clients[i] = client;
        break;
      }
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
    // remove client from storage
    for (int i = 0; i < 16; ++i)
      if (clients[i] == client)
        clients[i] = NULL;
  }
}