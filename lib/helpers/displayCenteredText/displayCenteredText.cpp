
#include <U8g2lib.h>
void displayCenteredText(U8G2 u8g2, const char *text)
{
    // Get the text width and height based on the font settings
    int textWidth = u8g2.getStrWidth(text);
    int textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    // Calculate x and y coordinates to center the text
    int x_coordinate = (128 - textWidth) / 2; // Assuming 128x64 display width
    int y_coordinate = (64 + textHeight) / 2; // Assuming 128x64 display height

    // Set the cursor to the calculated coordinates
    u8g2.setCursor(x_coordinate, y_coordinate);

    // Print the text on the display
    u8g2.print(text);
}
void displayCenteredTextAlongXAxis(U8G2 u8g2, const char *text, int y_coordinate)
{
    // Get the text width and height based on the font settings
    int textWidth = u8g2.getStrWidth(text);
    int textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    // Calculate x and y coordinates to center the text
    int x_coordinate = (128 - textWidth) / 2; // Assuming 128x64 display width

    // Set the cursor to the calculated coordinates
    u8g2.setCursor(x_coordinate, y_coordinate);

    // Print the text on the display
    u8g2.print(text);
}
void displayCenteredTextAlongYAxis(U8G2 u8g2, const char *text, int x_coordinate)
{
    // Get the text width and height based on the font settings
    int textWidth = u8g2.getStrWidth(text);
    int textHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

    // Calculate x and y coordinates to center the text
    int y_coordinate = (128 - textHeight) / 2; // Assuming 128x64 display width

    // Set the cursor to the calculated coordinates
    u8g2.setCursor(x_coordinate, y_coordinate);

    // Print the text on the display
    u8g2.print(text);
}
