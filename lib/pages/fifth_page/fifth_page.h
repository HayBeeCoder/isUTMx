#ifndef fifth_page_h
#define fifth_page_h
String EXTENSOMETER_RATING_IN_MM = "150";

#include <U8g2lib.h>
void fifth_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, int page, int page_value_address, char key, String test, int address, String &inputt_value, String &sensor_rating_in_kg, String &targetForce, int &selectedPage, String &targetExtension, int target_extension_address, String extensometer_rating);
#endif