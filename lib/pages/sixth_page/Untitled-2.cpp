// // In your fifth_page_ui function, in the page == 51 section, 
// // update the prompt based on test type:

// if (page == 51)
// {
//     u8g2.setFontMode(1);
//     u8g2.setDrawColor(1);
//     u8g2.drawBox(0, 10, 128, 12);
//     u8g2.setDrawColor(2);

//     u8g2.setFont(u8g2_font_4x6_tf);
//     String text = "LOADCELL RATING=";
//     u8g2.drawStr(2, 19, text.c_str());

//     char buffer[20];
//     int textSize = u8g2.getStrWidth(text.c_str());
//     formatForceValue(sensor_rating_in_kg.toFloat() * 9.81, buffer, sizeof(buffer));

//     u8g2.setFont(u8g2_font_5x7_tf);
//     u8g2.drawStr(textSize + 5, 19, buffer);

//     // Modify prompt based on test type
//     if (test.equalsIgnoreCase("tension") || test.equalsIgnoreCase("compression") || test.equalsIgnoreCase("bending"))
//     {
//         displayCenteredTextAlongXAxis(u8g2, "ENTER TARGET FORCE ", 32);
//         displayCenteredTextAlongXAxis(u8g2, "( in NEWTONS )", 42);
//     }
//     else if (test.equalsIgnoreCase("torsion"))
//     {
//         displayCenteredTextAlongXAxis(u8g2, "ENTER TARGET TORQUE ", 32);
//         displayCenteredTextAlongXAxis(u8g2, "( in N-m )", 42);
//     }   
// }

// // Similarly, in the page == 52 section, you can add bending handling:
// else if (page == 52)
// {
//     u8g2.setFontMode(1);
//     u8g2.setDrawColor(1);
//     u8g2.drawBox(0, 10, 128, 12);
//     u8g2.setDrawColor(2);
    
//     u8g2.setFont(u8g2_font_4x6_tf);
//     // Modify display based on test type
//     if (test.equalsIgnoreCase("tension") || test.equalsIgnoreCase("compression") || test.equalsIgnoreCase("bending"))
//     {
//         String text = "EXTENSOMETER RATING IN MM=";
//         u8g2.drawStr(2, 19, text.c_str());
//         int textSize = u8g2.getStrWidth(text.c_str());
//         u8g2.setFont(u8g2_font_5x7_tf);
//         u8g2.drawStr(textSize + 5, 19, extensometer_rating.c_str());

//         displayCenteredTextAlongXAxis(u8g2, "ENTER TARGET EXTENSION ", 32);
//         displayCenteredTextAlongXAxis(u8g2, "( in mm )", 42);
//     }
//     else if (test.equalsIgnoreCase("torsion"))
//     {
//         String text = "TORQUE SENSOR RATING=";
//         u8g2.drawStr(2, 19, text.c_str());
//         int textSize = u8g2.getStrWidth(text.c_str());
//         u8g2.setFont(u8g2_font_5x7_tf);
//         u8g2.drawStr(textSize + 5, 19, extensometer_rating.c_str());

//         displayCenteredTextAlongXAxis(u8g2, "ENTER ANGLE OF TWIST ", 32);
//         displayCenteredTextAlongXAxis(u8g2, "( in degrees )", 42);
//     }
// }