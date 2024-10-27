#include <U8g2lib.h>
#include <Wifi.h>
#include "wifi_connected.h"
#include "wifi_disconnected.h"
#define WIFI_PASSWORD "abcdefghij"
#define WIFI_SSID "isutmxx"

void check_wifi(U8G2_ST7920_128X64_F_SW_SPI u8g2)
{
    int currentWiFiStatus = WiFi.status();
    int wifiStatusValue = 0;
    if (currentWiFiStatus == WL_CONNECTED)
    {
        u8g2.drawXBMP(116, 52, u8g2_wifi_connected_width, u8g2_wifi_connected_height, u8g2_wifi_connected);
        wifiStatusValue = 1;
    }
    else
    {
        u8g2.drawXBMP(116, 52, u8g2_wifi_disconnected_width, u8g2_wifi_disconnected_height, u8g2_wifi_disconnected);
        wifiStatusValue = 0;
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}