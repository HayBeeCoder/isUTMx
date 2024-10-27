#include <check_wifi/check_wifi.h>
#include "wsEventHandler/wsEventHandler.h"
#include "displayCenteredText/displayCenteredText.h"
#include "formatForceValue/formatForceValue.h"
#include "formatExtensometerValue/formatExtensometerValue.h"
#define BUZZER_PIN 18
#include "EEPROM.h"
#include "types/status.h"

String serialData = "";
bool startBroadcast = false;
bool endBroadcast = false;
double temporary_force = -1;
float temporary_displacement = -1;
char buffer[20];
double force;
float displacement;
int period = 5000;
unsigned long time_now = 0;
Status current_status = NOT_STARTED;

const char *statusToString(Status &status)
{
    switch (status)
    {
    case NOT_STARTED:
        return "NOT STARTED";
    case STARTED:
        return "STARTED";
    case NEEDS_STOPPING:
        return "NEEDS STOPPING";
    case STOPPED:
        return "STOPPED";
    default:
        return "UNKNOWN_STATUS";
    }
}

// function decalarations
void box_with_white_background(U8G2_ST7920_128X64_F_SW_SPI u8g2, String sensor_rating, String target_force, String target_extension, String test);
void handle_key_pressed(U8G2_ST7920_128X64_F_SW_SPI u8g2, char &key, int current_page_address, int selected_page, String &target_force, String &sensor_rating, Status &current_status);
void handle_broadcast(float force, float displacement, String target_force);
void display_force_displacement(U8G2_ST7920_128X64_F_SW_SPI u8g2, float force, float displacement, String test);
void display_status_in_footer(U8G2_ST7920_128X64_F_SW_SPI u8g2, Status &status);
void handle_current_status_logic(Status &current_status, String &target_force, String &target_extension, char &key);

void process_serial2_data(String data)
{
    // Process the incoming serial data
    int commaIndex = data.indexOf(',');

    if (commaIndex > 0)
    {
        // Parse force and displacement values
        force = abs(data.substring(0, commaIndex).toFloat());
        displacement = data.substring(commaIndex + 1).toFloat();

        // You can now use the 'force' and 'displacement' values
    }
}

void check_serial2_data()
{
    // Check if there's data available on Serial2
    while (Serial2.available() > 0)
    {
        char receivedChar = Serial2.read(); // Read one character at a time

        // Append the character to the serialData string until '\n' is received
        if (receivedChar != '\n')
        {
            serialData += receivedChar;
        }
        else
        {
            // Once a full line is received, process the data
            process_serial2_data(serialData);
            serialData = ""; // Clear the buffer for the next data line
        }
    }
}

void sixth_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, char &key, String &test, String &sensor_rating, String &target_force, String &target_extension, int current_page_address, int selected_page)
{
    handle_current_status_logic(current_status, target_force, target_extension, key);
    handle_key_pressed(u8g2, key, current_page_address, selected_page, target_force, sensor_rating, current_status);
    check_serial2_data();
    u8g2.clearBuffer();
    check_wifi(u8g2);
    u8g2.setCursor(0, 7);
    u8g2.setFont(u8g2_font_5x7_tf);

    for (int i = 0; i < test.length(); i++)
    {
        test[i] = toupper(test[i]);
    }

    displayCenteredTextAlongXAxis(u8g2, ("TEST TYPE: " + test).c_str(), 7);

    // jkl;j
    box_with_white_background(u8g2, sensor_rating, target_force, target_extension, test);
    display_force_displacement(u8g2, force, displacement, test);

    display_status_in_footer(u8g2, current_status);

    u8g2.sendBuffer();
}

void box_with_white_background(U8G2_ST7920_128X64_F_SW_SPI u8g2, String sensor_rating, String target_force, String target_extension, String test)
{
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.drawBox(0, 10, 128, 19);
    u8g2.setDrawColor(2);

    u8g2.setFont(u8g2_font_4x6_tf);
    String text = "LOADCELL RATING=";
    u8g2.drawStr(2, 17, text.c_str());

    int textSize = u8g2.getStrWidth(text.c_str());
    formatForceValue(sensor_rating.toFloat() * 9.81, buffer, sizeof(buffer));

    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(textSize + 4, 17, buffer);

    u8g2.setFont(u8g2_font_4x6_tf);
    if (!target_force.isEmpty())
    {

        text = "TARGET FORCE=";
    }
    else if (!target_extension.isEmpty())
    {

        text = "TARGET EXTENSION=";
    }
    else
    {
        text = "TARGET FORCE=--   TARGET EXT=--";
    }
    u8g2.drawStr(2, 27, text.c_str());

    textSize = u8g2.getStrWidth(text.c_str());
    if (target_force.length() > 0)
    {
        double target_force_to_display = target_force.toFloat();

        if (test == "COMPRESSION")
        {
            target_force_to_display = target_force_to_display * -1;
        }
        formatForceValue(target_force_to_display, buffer, sizeof(buffer));
        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.drawStr(textSize + 4, 27, buffer);
    }
    else if (target_extension.length() > 0)
    {
        formatExtensometerValue(target_extension, buffer, sizeof(buffer));
        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.drawStr(textSize + 4, 27, buffer);
    }
    else
    {
        // u8g2.drawStr(textSize + 4, 27, "-----");
    }
    u8g2.setFont(u8g2_font_5x7_tf);
}

void handle_key_pressed(U8G2_ST7920_128X64_F_SW_SPI u8g2, char &key, int current_page_address, int selected_page, String &target_force, String &sensor_rating, Status &current_status)
{
    if (key == '0')
    {
        Serial.println("taring now");
        Serial2.print("taring");
        Serial2.println("\n");

        u8g2.clearBuffer();
        u8g2.setCursor(10, 39);
        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.print("TARING LOADCELL");
        u8g2.sendBuffer();
        delay(2000);
    }

    if (key == '*')
    {
        current_status = NOT_STARTED;
        broadcastStatus(NOT_STARTED);
        temporary_force = -1;
        Serial1.println("11");
        EEPROM.put(current_page_address, 51);

        // Make the program access memory for last value stored for target_force
        target_force = "-1";
        EEPROM.commit();
        EEPROM.get(current_page_address, selected_page);
    }

    if (key == '#')
    {

        if (current_status == NOT_STARTED)
        {
            current_status = STARTED;
            broadcastStatus(STARTED);
            temporary_force = -1;
            Serial1.println("11");
        }
        else if (current_status == STARTED)
        {
            current_status = STOPPED;
            broadcastStatus(STOPPED);
            // digitalWrite(BUZZER_PIN, HIGH);
            // delay(5000);
            // digitalWrite(BUZZER_PIN, LOW);
        }
        else if (current_status == STOPPED)
        {
            current_status = STARTED;
            digitalWrite(BUZZER_PIN, LOW);
            broadcastStatus(STARTED);

            Serial1.println("11");
        }
    }
}

void handle_broadcast(float force, float displacement, String target_force)
{
    ;
}

void display_force_displacement(U8G2_ST7920_128X64_F_SW_SPI u8g2, float force, float displacement, String test)
{
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(0, 40);
    u8g2.print("FORCE = ");
    if (test == "COMPRESSION")
    {
        force = force * -1;
    }

    u8g2.print(force, 1);
    u8g2.print(" N");

    u8g2.setCursor(0, 50);
    u8g2.print("EXTENSION = ");

    u8g2.print(displacement, 1);
    u8g2.print(" mm");
}

void display_status_in_footer(U8G2_ST7920_128X64_F_SW_SPI u8g2, Status &status)
{

    u8g2.setCursor(0, 64);
    u8g2.setFont(u8g2_font_4x6_tf);

    String displayText = "STATUS = " + String(statusToString(status));
    int16_t textWidth = u8g2.getStrWidth(displayText.c_str());
    int16_t displayWidth = u8g2.getDisplayWidth();

    int16_t x = (displayWidth - textWidth) / 2;

    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.drawBox(x - 5, u8g2.getDisplayHeight() - 9, textWidth + 10, 13);
    u8g2.setDrawColor(2);

    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawStr(x, u8g2.getDisplayHeight() - 2, displayText.c_str());
}

void handle_current_status_logic(Status &current_status, String &target_force, String &target_extension, char &key)
{

    switch (current_status)
    {
    case NOT_STARTED:
        /* code */
        break;
    case STARTED:
        if (temporary_force != force || floor(temporary_displacement) != floor(displacement))
        {
            if (target_force.isEmpty() && target_extension.isEmpty())

            {
                if (temporary_force > force && force == 0)
                {
                    broadcast_reading(force, displacement);

                    current_status = STOPPED;
                    broadcastStatus(STOPPED);
                    Serial1.println("10");
                    temporary_force = -1;
                }
                else
                {

                    broadcast_reading(force, displacement);
                    Serial1.println("11");
                    temporary_force = force;
                    temporary_displacement = displacement;
                }
            }
            else if (!target_force.isEmpty())
            {
                Serial.print("target_force: ");
                Serial.println(target_force);
                if (force < abs(target_force.toFloat()))
                {
                    broadcast_reading(force, displacement);
                    temporary_force = force;
                    temporary_displacement = displacement;
                    Serial1.println("11");
                }
                else
                {
                    current_status = STOPPED;
                    broadcast_reading(force, displacement);
                    broadcastStatus(STOPPED);
                    Serial1.println("10");
                }
            }
            else if (!target_extension.isEmpty())
            {
                if (abs(displacement) <= abs(target_extension.toFloat()))
                {
                    broadcast_reading(force, displacement);
                    temporary_force = force;
                    temporary_displacement = displacement;
                    Serial1.println("11");
                }
                else
                {
                    // digitalWrite(BUZZER_PIN, HIGH);
                    current_status = STOPPED;
                    broadcast_reading(force, displacement);
                    broadcastStatus(STOPPED);
                    // digitalWrite(BUZZER_PIN, LOW);
                    Serial1.println("10");
                }
            }
        }
        break;
    case NEEDS_STOPPING:
        /* code */
        break;
    case STOPPED:
        /* code */
        time_now = millis();
        digitalWrite(BUZZER_PIN, HIGH);

        while (millis() < time_now + period)

        {
            // delay(5000);
            // wait approx. [period] ms
            // if(key == '#'){
            //     current_status = STARTED;
            //     break;
            // }
        }
        digitalWrite(BUZZER_PIN, LOW);
        current_status = NOT_STARTED;
        break;

    default:
        break;
    }
}