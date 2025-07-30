#ifndef seventh_page_h
#define seventh_page_h
#include <U8g2lib.h>


void seventh_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, int page_value_address, char key, int &selected_page, float &calibrationFactorOutput);
#endif