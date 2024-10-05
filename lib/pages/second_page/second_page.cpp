#include <U8g2lib.h>
void second_page_ui(U8G2_ST7920_128X64_F_SW_SPI u8g2)
{

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB08_tf);
    //   int textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
    u8g2.setCursor(35, 24);
    u8g2.print("SUPERVISED");

    u8g2.setCursor(60, 37);
    u8g2.print("BY");

    u8g2.setCursor(10, 50);
    u8g2.print("PROF. A.T. AJIBOYE");

    u8g2.sendBuffer();
    delay(1000);
}
