// #include "fourth_page_one/fourth_page_one.h"
// #include "displayCenteredText/displayCenteredText.h"
// #include <EEPROM.h>

// // External definitions that should match main.cpp
// extern int SELECT_TEST_VALUE_ADDRESS;
// extern int SELECTED_PAGE_ADDRESS;

// enum SELECTED_TEST
// {
//   TENSION = 1,
//   COMPRESSION = 2,
//   TORSION = 3,
//   BENDING = 4
// };

// enum CURRENT_PAGE
// {
//   FIRST = 1,
//   SECOND = 2,
//   THIRD = 3,
//   FOURTH = 4,
//   FOURTH_ONE = 41, // Compression subpage
//   FIFTH = 5,
//   FIFTH_ONE = 51,
//   FIFTH_TWO = 52,
//   FIFTH_THREE = 53,
//   SIXTH = 6,
//   SIXTH_ONE = 61,
//   SEVENTH = 7
// };

// void fourth_page_one_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, char key, int current_page_address, int selected_page)
// {
//     // Clear the display buffer
//     u8g2.clearBuffer();
    
//     // Set font for the header
//     u8g2.setFont(u8g2_font_6x10_tf);
    
//     // Display the main header
//     displayCenteredTextAlongXAxis(u8g2, "SELECT COMPRESSION TYPE", 10);
    
//     // Set font for the options
//     u8g2.setFont(u8g2_font_5x7_tf);
    
//     // Display the test options
//     u8g2.setCursor(5, 25);
//     u8g2.print("1. COMPRESSION TEST");
    
//     u8g2.setCursor(5, 36);
//     u8g2.print("2. BENDING TEST");
    
//     // Draw a separator line
//     u8g2.drawLine(5, 42, 123, 42);
    
//     // Display instructions
//     u8g2.setFont(u8g2_font_4x6_tf);
//     u8g2.setCursor(5, 52);
//     u8g2.print("HOLD 1 FOR COMPRESSION");
    
//     u8g2.setCursor(5, 60);
//     u8g2.print("HOLD 2 FOR BENDING");
    
//     // Display back option at the bottom
//     u8g2.setCursor(5, 64);
//     u8g2.print("PRESS * TO GO BACK");
    
//     // Send buffer to display
//     u8g2.sendBuffer();
// }

// void handle_fourth_page_one_input(char key, U8G2_ST7920_128X64_F_SW_SPI u8g2)
// {
//     static unsigned long keyHoldStartTime = 0;
//     static char lastPressedKey = 0;
//     static bool keyHeld = false;
//     const unsigned long HOLD_DURATION = 1000; // 1 second hold time
    
//     if (key != 0) // Key is pressed
//     {
//         if (key != lastPressedKey) // New key pressed
//         {
//             keyHoldStartTime = millis();
//             lastPressedKey = key;
//             keyHeld = false;
            
//             // Show immediate feedback for key press
//             if (key == '1')
//             {
//                 show_selection_feedback(u8g2, "COMPRESSION SELECTED", "HOLD TO CONFIRM...");
//             }
//             else if (key == '2')
//             {
//                 show_selection_feedback(u8g2, "BENDING SELECTED", "HOLD TO CONFIRM...");
//             }
//             else if (key == '*')
//             {
//                 // Immediate back action - no hold required
//                 EEPROM.put(SELECTED_PAGE_ADDRESS, FOURTH);
//                 EEPROM.commit();
//                 return;
//             }
//         }
//         else // Same key still being held
//         {
//             if (!keyHeld && (millis() - keyHoldStartTime >= HOLD_DURATION))
//             {
//                 keyHeld = true;
                
//                 // Process the held key
//                 if (key == '1')
//                 {
//                     // Select compression test
//                     EEPROM.put(SELECT_TEST_VALUE_ADDRESS, COMPRESSION);
//                     EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
//                     EEPROM.commit();
                    
//                     // Send test type to Arduino Nano
//                     Serial2.println("compression");
                    
//                     // Show confirmation
//                     show_selection_feedback(u8g2, "COMPRESSION TEST", "CONFIRMED!");
//                     delay(1000);
//                 }
//                 else if (key == '2')
//                 {
//                     // Select bending test
//                     EEPROM.put(SELECT_TEST_VALUE_ADDRESS, BENDING);
//                     EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
//                     EEPROM.commit();
                    
//                     // Send test type to Arduino Nano
//                     Serial2.println("bending");
                    
//                     // Show confirmation
//                     show_selection_feedback(u8g2, "BENDING TEST", "CONFIRMED!");
//                     delay(1000);
//                 }
//             }
//         }
//     }
//     else // No key pressed
//     {
//         lastPressedKey = 0;
//         keyHeld = false;
//     }
// }

// void show_selection_feedback(U8G2_ST7920_128X64_F_SW_SPI u8g2, const char* title, const char* message)
// {
//     u8g2.clearBuffer();
    
//     // Draw a border box
//     u8g2.drawFrame(10, 15, 108, 35);
//     u8g2.drawFrame(11, 16, 106, 33);
    
//     // Set font and display title
//     u8g2.setFont(u8g2_font_6x10_tf);
//     displayCenteredTextAlongXAxis(u8g2, title, 28);
    
//     // Set smaller font for message
//     u8g2.setFont(u8g2_font_5x7_tf);
//     displayCenteredTextAlongXAxis(u8g2, message, 40);
    
//     u8g2.sendBuffer();
// }

// void show_hold_progress(U8G2_ST7920_128X64_F_SW_SPI u8g2, const char* test_name, unsigned long elapsed_time, unsigned long required_time)
// {
//     u8g2.clearBuffer();
    
//     // Calculate progress percentage
//     int progress = (elapsed_time * 100) / required_time;
//     if (progress > 100) progress = 100;
    
//     // Display test name
//     u8g2.setFont(u8g2_font_6x10_tf);
//     displayCenteredTextAlongXAxis(u8g2, test_name, 15);
    
//     // Display progress text
//     u8g2.setFont(u8g2_font_5x7_tf);
//     displayCenteredTextAlongXAxis(u8g2, "HOLD TO CONFIRM", 28);
    
//     // Draw progress bar background
//     u8g2.drawFrame(20, 35, 88, 10);
    
//     // Draw progress bar fill
//     int fill_width = (progress * 86) / 100;
//     if (fill_width > 0)
//     {
//         u8g2.drawBox(21, 36, fill_width, 8);
//     }
    
//     // Display percentage
//     char percent_text[10];
//     sprintf(percent_text, "%d%%", progress);
//     displayCenteredTextAlongXAxis(u8g2, percent_text, 55);
    
//     u8g2.sendBuffer();
// }

// // Alternative implementation with visual progress feedback
// void handle_fourth_page_one_input_with_progress(char key, U8G2_ST7920_128X64_F_SW_SPI u8g2)
// {
//     static unsigned long keyHoldStartTime = 0;
//     static char lastPressedKey = 0;
//     static bool keyHeld = false;
//     const unsigned long HOLD_DURATION = 1500; // 1.5 second hold time
    
//     if (key != 0) // Key is pressed
//     {
//         if (key != lastPressedKey) // New key pressed
//         {
//             keyHoldStartTime = millis();
//             lastPressedKey = key;
//             keyHeld = false;
//         }
//         else // Same key still being held
//         {
//             unsigned long elapsed = millis() - keyHoldStartTime;
            
//             if (!keyHeld && elapsed < HOLD_DURATION)
//             {
//                 // Show progress while holding
//                 if (key == '1')
//                 {
//                     show_hold_progress(u8g2, "COMPRESSION TEST", elapsed, HOLD_DURATION);
//                 }
//                 else if (key == '2')
//                 {
//                     show_hold_progress(u8g2, "BENDING TEST", elapsed, HOLD_DURATION);
//                 }
//             }
//             else if (!keyHeld && elapsed >= HOLD_DURATION)
//             {
//                 keyHeld = true;
                
//                 // Process the held key
//                 if (key == '1')
//                 {
//                     // Select compression test
//                     EEPROM.put(SELECT_TEST_VALUE_ADDRESS, COMPRESSION);
//                     EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
//                     EEPROM.commit();
                    
//                     // Send test type to Arduino Nano
//                     Serial2.println("compression");
                    
//                     // Show confirmation
//                     show_selection_feedback(u8g2, "COMPRESSION TEST", "CONFIRMED!");
//                     delay(1000);
//                 }
//                 else if (key == '2')
//                 {
//                     // Select bending test
//                     EEPROM.put(SELECT_TEST_VALUE_ADDRESS, BENDING);
//                     EEPROM.put(SELECTED_PAGE_ADDRESS, FIFTH);
//                     EEPROM.commit();
                    
//                     // Send test type to Arduino Nano
//                     Serial2.println("bending");
                    
//                     // Show confirmation
//                     show_selection_feedback(u8g2, "BENDING TEST", "CONFIRMED!");
//                     delay(1000);
//                 }
//             }
//         }
        
//         // Handle immediate back action
//         if (key == '*')
//         {
//             EEPROM.put(SELECTED_PAGE_ADDRESS, FOURTH);
//             EEPROM.commit();
//             lastPressedKey = 0;
//             keyHeld = false;
//         }
//     }
//     else // No key pressed
//     {
//         lastPressedKey = 0;
//         keyHeld = false;
//     }
// }