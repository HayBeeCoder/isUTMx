// #ifndef FOURTH_PAGE_ONE_H
// #define FOURTH_PAGE_ONE_H

// #include <Arduino.h>
// #include <U8g2lib.h>

// /**
//  * Display the compression subpage UI showing options for compression and bending tests
//  * @param u8g2 Display object
//  * @param key Currently pressed key
//  * @param current_page_address EEPROM address for current page
//  * @param selected_page Current selected page value
//  */
// void fourth_page_one_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, char key, int current_page_address, int selected_page);

// /**
//  * Handle user input for the fourth page one (compression subpage)
//  * Basic implementation with hold-to-confirm functionality
//  * @param key Currently pressed key
//  * @param u8g2 Display object
//  */
// void handle_fourth_page_one_input(char key, U8G2_ST7920_128X64_F_SW_SPI u8g2);

// /**
//  * Handle user input with visual progress feedback while holding keys
//  * Enhanced implementation showing progress bar
//  * @param key Currently pressed key
//  * @param u8g2 Display object
//  */
// void handle_fourth_page_one_input_with_progress(char key, U8G2_ST7920_128X64_F_SW_SPI u8g2);

// /**
//  * Show selection feedback to user
//  * @param u8g2 Display object
//  * @param title Title text to display
//  * @param message Message text to display
//  */
// void show_selection_feedback(U8G2_ST7920_128X64_F_SW_SPI u8g2, const char* title, const char* message);

// /**
//  * Show progress while user is holding a key
//  * @param u8g2 Display object
//  * @param test_name Name of the test being selected
//  * @param elapsed_time Time elapsed since key press started
//  * @param required_time Total time required to hold key
//  */
// void show_hold_progress(U8G2_ST7920_128X64_F_SW_SPI u8g2, const char* test_name, unsigned long elapsed_time, unsigned long required_time);

// #endif // FOURTH_PAGE_ONE_H