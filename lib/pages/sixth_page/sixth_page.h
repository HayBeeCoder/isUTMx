#ifndef sixth_page_h
#define sixth_page_h


#include <U8g2lib.h>

void sixth_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, char &key, String test, String &sensor_rating, String &target_force, int current_page_address, int selected_page);
#endif