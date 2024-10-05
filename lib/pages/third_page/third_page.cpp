#include <U8g2lib.h>
#include <WiFi.h>
#include <mdns.h>

const char *text = "WI-FI CONNECTING ...";
const char *hostname = "isutmx";
String address;

void start_mdns_service()
{
    mdns_init();
    mdns_hostname_set(hostname);
    mdns_instance_name_set("Instrumentation System for universal testing machine");
}

void third_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, const char *ssid, const char *password)
{
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.clearBuffer();

    int textWidth = u8g2.getStrWidth(text);
    int textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    int x_coordinate = (128 - textWidth) / 2;
    int y_coordinate = (64 + textHeight) / 2;

    WiFi.disconnect(true);

    // Set the hostname before connecting to WiFi
    // Set the desired hostname

    WiFi.begin(ssid, password);
    start_mdns_service();

    u8g2.setCursor(x_coordinate, y_coordinate);
    u8g2.print(text);
    u8g2.sendBuffer();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
    }

    u8g2.clearBuffer();

    text = "WI-FI CONNECTED! ;)";
    textWidth = u8g2.getStrWidth(text);
    textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    x_coordinate = (128 - textWidth) / 2;
    y_coordinate = (64 + textHeight) / 2;

    u8g2.setCursor(x_coordinate, y_coordinate);
    u8g2.print(text);

    u8g2.setFont(u8g2_font_tom_thumb_4x6_tr);

    address = "http://" + String(hostname) + ".local";

    textWidth = u8g2.getStrWidth(address.c_str());
    textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    x_coordinate = (128 - textWidth) / 2;
    // y_coordinate = (64 + textHeight) / 2;
    // Serial.println(WiFi.localIP());
    u8g2.setCursor(x_coordinate, y_coordinate + 10);
    u8g2.print(address);
    u8g2.sendBuffer();
    delay(2000);
}