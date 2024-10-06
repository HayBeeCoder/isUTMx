#ifndef _WS_EVENT_HANDLER_H_
#define _WS_EVENT_HANDLER_H_
#include <ESPAsyncWebServer.h>

#include "types/status.h"
void broadcast_reading(double force, double displacement);
void broadcastPage(String page);
void broadcastSixthPageInfo(String test_type, String sensor_rating, String target_force,String target_extension) ;
void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void broadcastStatus(Status status);
#endif