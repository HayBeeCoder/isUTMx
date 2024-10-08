
#include <U8g2lib.h>
#include <check_wifi/check_wifi.h>
#include <EEPROM.h>
#include "displayCenteredText/displayCenteredText.h"
#include "formatForceValue/formatForceValue.h"

bool cursorVisible = true;
const long interval = 500;        // Blink interval (500 ms)
unsigned long previousMillis = 0; // For the blinking effect
int MAX_LENGTH_OF_INPUT_VALUE_ALLOWED = 9;
void fifth_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, int page, int page_value_address, char key, String test, int address, String &inputt_value, String &sensor_rating_in_kg, String &targetForce, int &selected_page, String &targetExtension, int target_extension_address, String extensometer_rating)
{

    u8g2.clearBuffer();
    check_wifi(u8g2);
    u8g2.setFont(u8g2_font_5x7_tf);

    for (int i = 0; i < test.length(); i++)
    {
        test[i] = toupper(test[i]);
    }

    displayCenteredTextAlongXAxis(u8g2, ("TEST TYPE: " + test).c_str(), 7);

    if (page == 5)
    {

        displayCenteredTextAlongXAxis(u8g2, "KINDLY ENTER LOADCELL'S", 27);
        displayCenteredTextAlongXAxis(u8g2, "RATING IN KG", 37);
    }
    else if (page == 51)
    {

        u8g2.setFontMode(1);
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 10, 128, 12);
        u8g2.setDrawColor(2);

        u8g2.setFont(u8g2_font_4x6_tf);
        String text = "LOADCELL RATING=";
        u8g2.drawStr(2, 19, text.c_str());

        char buffer[20];
        int textSize = u8g2.getStrWidth(text.c_str());
        formatForceValue(sensor_rating_in_kg.toFloat() * 9.81, buffer, sizeof(buffer));

        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.drawStr(textSize + 5, 19, buffer);

        displayCenteredTextAlongXAxis(u8g2, "ENTER TARGET FORCE ", 32);
        displayCenteredTextAlongXAxis(u8g2, "( in NEWTONS )", 42);
    }
    else if (page == 52)
    {

        u8g2.setFontMode(1);
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 10, 128, 12);
        u8g2.setDrawColor(2);

        u8g2.setFont(u8g2_font_4x6_tf);
        String text = "EXTENSOMETER RATING IN MM=";
        u8g2.drawStr(2, 19, text.c_str());
        int textSize = u8g2.getStrWidth(text.c_str());
        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.drawStr(textSize + 5, 19, extensometer_rating.c_str());

        displayCenteredTextAlongXAxis(u8g2, "ENTER TARGET EXTENSION ", 32);
        displayCenteredTextAlongXAxis(u8g2, "( in mm )", 42);
    }

    if (key)
    {

        if (key == '*')
        {
            // if(inputt)
            Serial.print("inputt value is :  ");
            Serial.println(inputt_value);
            if (inputt_value == "")
            {
                // Goes to previous page
                Serial.print("current page is : ");
                Serial.println(page);
                if (page == 5)
                {

                    EEPROM.put(page_value_address, 4);
                }else if (page == 51)
                {
                    EEPROM.put(page_value_address, 5);
                    sensor_rating_in_kg = "-1";
                    targetForce = "-1";
                }else if (page == 52)
                {
                    EEPROM.put(page_value_address, 51);
                    targetForce = "-1";
                    targetExtension = "-1";
                }
                EEPROM.commit();
                EEPROM.get(page_value_address, selected_page);
            }
            else
            {

                inputt_value = inputt_value.substring(0, inputt_value.length() - 1);
            }
        }
        else if (key == '#')
        {

            //  5 implies first sub page , 51 implies next subpage
            double SENSOR_RATING_IN_N = sensor_rating_in_kg.toFloat() * 9.81;
            double LOADCELL_TOLERANCE = 0.01 * SENSOR_RATING_IN_N;
            int EXTENSOMETER_TOLERANCE = 5;
            EEPROM.commit();
            // Proceed to next page only if at least one of the following conditions is true:
            // 1. Current page is 5 and the sensor_rating_in_kg variable is non-empty
            // 2. Current page is 51 and the targetForce value is less than the sensor rating
            EEPROM.put(address, inputt_value);

            if (
                (page == 5 &&
                 !inputt_value.isEmpty()) ||
                (page == 51 &&
                 ((SENSOR_RATING_IN_N - inputt_value.toFloat()) >= LOADCELL_TOLERANCE)) ||
                (page == 52 &&
                 ((extensometer_rating.toInt() - inputt_value.toInt()) >= EXTENSOMETER_TOLERANCE)))
            {
                if (page == 5)
                {
                    EEPROM.put(page_value_address, 51);
                }
                else if (page == 51)
                {
                    if (!inputt_value.isEmpty())
                    {
                        EEPROM.put(target_extension_address, "");
                        EEPROM.put(page_value_address, 6);
                    }
                    else if (inputt_value.isEmpty())
                    {
                        EEPROM.put(page_value_address, 52);
                    }
                }
                else if (page == 52)
                {
                    EEPROM.put(page_value_address, 6);
                }
                EEPROM.commit();
                if (page == 5)
                {

                    EEPROM.get(address, sensor_rating_in_kg);
                }
                else if (page == 51)
                {

                    EEPROM.get(address, targetForce);
                }
                else if (page == 52)
                {
                    EEPROM.get(address, targetExtension);
                }
                inputt_value = "";
            }
            // if (pageType == SENSOR_RATING_PAGE) {
            // isSetSensorRatingPage = false;
            // isSetTargetForcePage = true;
            // } else if (pageType == TARGET_FORCE_PAGE) {
            // isSetTargetForcePage = false;
            // }
        }
        else
        {

            if (inputt_value.length() <= MAX_LENGTH_OF_INPUT_VALUE_ALLOWED)
            {

                inputt_value += key;
            }
        }
    }
    int textWidth = u8g2.getStrWidth(inputt_value.c_str());
    u8g2.drawStr((128 - textWidth) / 2, 52, inputt_value.c_str());
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        cursorVisible = !cursorVisible;
    }
    if (cursorVisible)
    {
        int cursorX = ((128 - textWidth) / 2) + textWidth;
        int cursorY = 52;
        u8g2.drawLine(cursorX, cursorY + 2, cursorX + 6, cursorY + 2);
    }
    // inputFn(key, sensor_rating_in_kg, !isAskTestPage && !isTestPage && key, SENSOR_RATING_PAGE);
    u8g2.sendBuffer();
}
