
#include <U8g2lib.h>
#ifndef display_centered_text_h
#define display_centered_text_h

void displayCenteredText(U8G2 u8g2, const char *text);
void displayCenteredTextAlongXAxis(U8G2 u8g2, const char *text, int y_coordinate);
void displayCenteredTextAlongYAxis(U8G2 u8g2, const char *text, int x_coordinate);
#endif