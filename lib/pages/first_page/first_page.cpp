#include <U8g2lib.h>

void first_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2)
{

    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.clearBuffer();
    u8g2.setCursor(9, 7);
    u8g2.print("INSTRUMENTATION SYSTEM");
    u8g2.setCursor(14, 17);
    u8g2.print("FOR MATERIAL TESTING");
    u8g2.setCursor(39, 27);
    u8g2.print("MACHINE BY");

    u8g2.setCursor(9, 37);
    u8g2.print("OGUNTADE A. 18/30GR043");

    u8g2.setCursor(16, 47);
    u8g2.print("ALADE T. 18/30GR023");

    u8g2.setCursor(9, 57);
    u8g2.print("AGUNSOYE E. 18/30GR016");
    u8g2.sendBuffer();
    delay(1000);
}
