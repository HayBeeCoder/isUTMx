#include "HX711.h"
#include <SoftwareSerial.h>
#include <math.h>

// Define custom RX and TX pins for SoftwareSerial
SoftwareSerial mySerial(7, 6);  // (TX, RX)serial.
HX711 scale;


/*Connection port for the sensors
+------------------------------------------+
+             torsion loadcell             +
+------------------------------------------+
+       tesion and compression loadcell    +                                     +
+------------------------------------------+
+           angle sensor                   +
+------------------------------------------+
+          extensometer                    +
+------------------------------------------+
*/
// This is for tension and compression
// #define LOADCELL_DOUT_PIN 3
// #define LOADCELL_SCK_PIN 2

// This is for tension/compression load cell
#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5

#define VERNIER_CLOCK_PIN 9
#define VERNIER_DATA_PIN 8

float readCaliper();
// calibration factor
float calibration_factor = -22.9926536;
// float calibration_factor = -7050;
long zero_factor;
char buf[20];
unsigned long tmpTime;
String tare;
int sign;
int inches;
long value;
float result;
bool mm = true;                //define mm to false if you want inches values
unsigned long timeout = 1000;  // Timeout in microseconds
bool s_load_cell_connected = false;
char outputBuffer[10];  // Buffer to hold the final string


void setup() {
  mySerial.begin(4800);
  Serial.begin(115200);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  pinMode(VERNIER_CLOCK_PIN, INPUT);
  pinMode(VERNIER_DATA_PIN, INPUT);

  // CALIBRATION VALUES
  scale.set_offset(4294962314);
  scale.set_scale(calibration_factor);

  scale.tare();                        //Reset the scale to 0
  zero_factor = scale.read_average();  //Get a baseline reading
}

char TORSION[] = "torsion";
char COMPRESSION[] = "compression";
char TENSION[] = "tension";
char RESET[] = "reset";
String SELECTED;

void loop() {

  // readCaliper();
  if (mySerial.available()) {
    tare = mySerial.readString();

    tare.trim();
    Serial.print("RECEIVED: ");
    Serial.println(tare);
    if (tare == "taring") {
      Serial.println(tare);
      scale.tare();
    }
  }

  scale.set_scale(calibration_factor);  //Adjust to this calibration factor
  float weight = scale.get_units(1);
  // this weight is in grams
  Serial.print("weight: ");
  weight = floor(weight * 10) / 10;


  Serial.println(weight);
  // Convert weight from grammes to kilogrammes to Newtons and take the absolute value
  float weightInNewtons = (weight / 1000) * 9.81 * -1;  // Convert from pounds to kilograms to Newtons
  weightInNewtons = floor(weightInNewtons * 10) / 10;
  if (weightInNewtons == 0.0 || weightInNewtons == -0.0 || weightInNewtons == -0.1) {
    weightInNewtons = 0.0;
  }

  int myInt = (int)round(weightInNewtons);  // truncate the decimal part of the weightinNewtons value
  if (abs(myInt) == 0 || abs(myInt) == 1) {
    myInt = 0;
  }

  Serial.println("weightInNewtons : ");  // readCaliper()
  Serial.println(weightInNewtons);       // readCaliper()
  // Serial.println("\n");
  mySerial.print((float)myInt);   // Send weight
  mySerial.print(",");            // Add a delimiter
  mySerial.print(readCaliper());  // Send caliper reading
  mySerial.println();             // End the message with a newline
}


float readCaliper() {
  unsigned long timeout = millis() + 1000;  // Timeout after 1 second

  // Wait for clock pin to go LOW, with timeout
  while (digitalRead(VERNIER_CLOCK_PIN) == LOW) {
    if (millis() > timeout) {
      // Serial.println("Timeout: No clock signal (Vernier may be off)");
      return 0.0;  // Return a default value indicating no data
    }
  }

  tmpTime = micros();

  // Wait for clock pin to go HIGH, with timeout
  while (digitalRead(VERNIER_CLOCK_PIN) == HIGH) {
    if (millis() > timeout) {
      // Serial.println("Timeout: No clock signal (Vernier may be off)");
      return 0.0;  // Return a default value indicating no data
    }
  }

  // Check if the signal interval is too short
  if ((micros() - tmpTime) < 500) {
    return 0.0;  // Return a default value indicating an error or no valid data
  }

  sign = 1;
  value = 0;
  inches = 0;

  for (int i = 0; i < 24; i++) {
    while (digitalRead(VERNIER_CLOCK_PIN) == LOW) {
      if (millis() > timeout) {
        return 0.0;
      }
    }

    // Wait for clock HIGH with timeout
    while (digitalRead(VERNIER_CLOCK_PIN) == HIGH) {
      if (millis() > timeout) {
        // Serial.println("Timeout: No clock signal during data read");
        return 0.0;
      }
    }

    // Read data pin
    if (digitalRead(VERNIER_DATA_PIN) == HIGH) {
      if (i < 20) value |= (1 << i);
      if (i == 20) sign = -1;
      if (i == 23) inches = 1;
    }
  }


  if (mm) {
    result = (value * sign) / 100.0;
  } else {
    result = (value * sign) / (inches ? 2000.0 : 100.0);  // Map values for inches
  }


  result = (floor(result * 10) / 10);

  return result;
}
