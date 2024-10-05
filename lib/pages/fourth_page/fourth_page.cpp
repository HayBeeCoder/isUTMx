#include <U8g2lib.h>
#include <EEPROM.h>

void  fourth_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2, int address, int value_to_store_at_address) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(2, 7);
  u8g2.print("SELECT TEST TO BE CARRIED");
  u8g2.setCursor(58, 18);
  u8g2.print("OUT");
  u8g2.setCursor(3, 34);
  u8g2.print("HOLD 1 DOWN FOR TENSION");
  u8g2.setCursor(3, 45);
  u8g2.print("HOLD 2 FOR COMPRESSION");
  u8g2.setCursor(3, 56);
  u8g2.print("HOLD 3 DOWN FOR T0RSION");
  EEPROM.put(address, value_to_store_at_address);
  EEPROM.commit();
  EEPROM.get(address, value_to_store_at_address);
  u8g2.sendBuffer();
}
