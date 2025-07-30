#include <U8g2lib.h>
#include <check_wifi/check_wifi.h>
#include <EEPROM.h>
#include "displayCenteredText/displayCenteredText.h"

enum CALIBRATION_STATE
{
    READING_BASELINE = 1,
    ENTER_KNOWN_WEIGHT = 2,
    APPLY_WEIGHT = 3,
    CALCULATING_FACTOR = 4,
    DISPLAY_FACTOR = 5,
    CALIBRATION_COMPLETE = 6
};

static bool cursorVisible = true;
static const long interval = 500;        
static unsigned long previousMillis = 0; 
static int MAX_LENGTH_OF_INPUT_VALUE_ALLOWED = 9;

// Static variables to maintain state across function calls
static int calibrationState = READING_BASELINE;
static float baselineReading = 0.0;
static float currentReading = 0.0;
static float knownWeight = 0.0;
static float calibrationFactor = 0.0;
static unsigned long displayStartTime = 0;
static bool factorCaptured = false;
static String inputValue = "";
// Move these three lines from inside the function to here (with other static variables)
static String serialDataCalibration = "";
static double forceCalibration = 0.0;
static float displacementCalibration = 0.0;

void seventh_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, int page_value_address, char key, int &selected_page, float &calibrationFactorOutput)
{
    // Add this serial data checking
    while (Serial2.available() > 0)
    {
        char receivedChar = Serial2.read();
        if (receivedChar != '\n')
        {
            serialDataCalibration += receivedChar;
        }
        else
        {
            int commaIndex = serialDataCalibration.indexOf(',');
            if (commaIndex > 0)
            {
                forceCalibration = abs(serialDataCalibration.substring(0, commaIndex).toFloat());
                displacementCalibration = serialDataCalibration.substring(commaIndex + 1).toFloat();
            }
            serialDataCalibration = "";
        }
    }
    u8g2.clearBuffer();
    check_wifi(u8g2);
    u8g2.setFont(u8g2_font_5x7_tf);

    displayCenteredTextAlongXAxis(u8g2, "LOAD CELL CALIBRATION", 7);

    switch(calibrationState) 
    {
        case READING_BASELINE:
        {
            displayCenteredTextAlongXAxis(u8g2, "REMOVE ALL WEIGHTS", 20);
            displayCenteredTextAlongXAxis(u8g2, "FROM LOAD CELL", 30);
            
            // Display current reading
            char readingBuffer[20];
            sprintf(readingBuffer, "READING: %.2f", forceCalibration);
            displayCenteredTextAlongXAxis(u8g2, readingBuffer, 42);
            
            displayCenteredTextAlongXAxis(u8g2, "PRESS # TO CONTINUE", 55);
            
            if (key == '#') {
                baselineReading = forceCalibration;
                calibrationState = ENTER_KNOWN_WEIGHT;
                inputValue = "";
            }
            break;
        }
        
        case ENTER_KNOWN_WEIGHT:
        {
            displayCenteredTextAlongXAxis(u8g2, "ENTER KNOWN WEIGHT", 20);
            displayCenteredTextAlongXAxis(u8g2, "(in kg)", 30);
            
            // Handle input
            if (key) {
                if (key == '*') {
                    if (inputValue == "") {
                        calibrationState = READING_BASELINE;
                    } else {
                        inputValue = inputValue.substring(0, inputValue.length() - 1);
                    }
                }
                else if (key == '#') {
                    if (!inputValue.isEmpty() && inputValue.toFloat() > 0) {
                        knownWeight = inputValue.toFloat();
                        calibrationState = APPLY_WEIGHT;
                    }
                }
                else {
                    if (inputValue.length() <= MAX_LENGTH_OF_INPUT_VALUE_ALLOWED) {
                        inputValue += key;
                    }
                }
            }
            
            // Display input with cursor
            int textWidth = u8g2.getStrWidth(inputValue.c_str());
            u8g2.drawStr((128 - textWidth) / 2, 45, inputValue.c_str());
            
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
                previousMillis = currentMillis;
                cursorVisible = !cursorVisible;
            }
            if (cursorVisible) {
                int cursorX = ((128 - textWidth) / 2) + textWidth;
                int cursorY = 45;
                u8g2.drawLine(cursorX, cursorY + 2, cursorX + 6, cursorY + 2);
            }
            
            displayCenteredTextAlongXAxis(u8g2, "PRESS # TO CONFIRM", 55);
            break;
        }
        
        case APPLY_WEIGHT:
        {
            char weightBuffer[30];
            sprintf(weightBuffer, "APPLY %.2f kg WEIGHT", knownWeight);
            displayCenteredTextAlongXAxis(u8g2, weightBuffer, 20);
            displayCenteredTextAlongXAxis(u8g2, "TO THE LOAD CELL", 30);
            
            // Display current reading
            char readingBuffer[20];
            sprintf(readingBuffer, "READING: %.2f", forceCalibration);
            displayCenteredTextAlongXAxis(u8g2, readingBuffer, 42);
            
            displayCenteredTextAlongXAxis(u8g2, "PRESS # WHEN STABLE", 55);
            
            if (key == '#') {
                currentReading = forceCalibration;
                calibrationState = CALCULATING_FACTOR;
            } else if (key == '*') {
                calibrationState = ENTER_KNOWN_WEIGHT;
            }
            break;
        }
        
        case CALCULATING_FACTOR:
        {
            displayCenteredTextAlongXAxis(u8g2, "CALCULATING...", 30);
            
            // Calculate calibration factor
            float readingDifference = currentReading - baselineReading;
            float knownForce = knownWeight * 9.81; // Convert kg to Newtons
            
            if (readingDifference != 0) {
                calibrationFactor = knownForce / readingDifference;
                calibrationState = DISPLAY_FACTOR;
                displayStartTime = millis();
                factorCaptured = false;
            } else {
                displayCenteredTextAlongXAxis(u8g2, "ERROR: NO CHANGE", 40);
                displayCenteredTextAlongXAxis(u8g2, "IN READING", 50);
                calibrationState = APPLY_WEIGHT;
            }
            break;
        }
        
        case DISPLAY_FACTOR:
        {
            displayCenteredTextAlongXAxis(u8g2, "CALIBRATION FACTOR:", 20);
            
            char factorBuffer[20];
            sprintf(factorBuffer, "%.6f", calibrationFactor);
            displayCenteredTextAlongXAxis(u8g2, factorBuffer, 32);
            
            // Capture factor for 5 seconds
            if (!factorCaptured) {
                calibrationFactorOutput = calibrationFactor;
                factorCaptured = true;
            }
            
            unsigned long elapsed = millis() - displayStartTime;
            char timeBuffer[20];
            sprintf(timeBuffer, "TIME: %lu/5000 ms", elapsed);
            displayCenteredTextAlongXAxis(u8g2, timeBuffer, 44);
            
            if (elapsed >= 5000) {
                calibrationState = CALIBRATION_COMPLETE;
            }
            
            displayCenteredTextAlongXAxis(u8g2, "PRESS * TO RESTART", 56);
            
            if (key == '*') {
                calibrationState = READING_BASELINE;
                inputValue = "";
                factorCaptured = false;
            }
            break;
        }
        
        case CALIBRATION_COMPLETE:
        {
            displayCenteredTextAlongXAxis(u8g2, "CALIBRATION COMPLETE!", 20);
            
            char factorBuffer[20];
            sprintf(factorBuffer, "FACTOR: %.6f", calibrationFactor);
            displayCenteredTextAlongXAxis(u8g2, factorBuffer, 32);
            
            displayCenteredTextAlongXAxis(u8g2, "PRESS # TO SAVE & EXIT", 44);
            displayCenteredTextAlongXAxis(u8g2, "PRESS * TO RECALIBRATE", 56);
            
            if (key == '#') {
                // Save calibration factor to EEPROM if needed
                // EEPROM.put(CALIBRATION_FACTOR_ADDRESS, calibrationFactor);
                // EEPROM.commit();
                
                // Return to previous page or main menu
                EEPROM.put(page_value_address, 32); // Go back to THIRD_TWO
                EEPROM.commit();
                EEPROM.get(page_value_address, selected_page);
                
                // Reset calibration state for next time
                calibrationState = READING_BASELINE;
                inputValue = "";
                factorCaptured = false;
            } else if (key == '*') {
                calibrationState = READING_BASELINE;
                inputValue = "";
                factorCaptured = false;
            }
            break;
        }
    }
    
    u8g2.sendBuffer();
}