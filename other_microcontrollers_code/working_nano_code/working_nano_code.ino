#include "HX711.h"
#include <SoftwareSerial.h>
#include <math.h>
#include <Wire.h>

// AS5600 I2C address
#define AS5600_ADDRESS 0x36
#define RAW_ANGLE_REGISTER 0x0C

// Define custom RX and TX pins for SoftwareSerial
SoftwareSerial mySerial(7, 6);  // (TX, RX)serial.

// Load cell instances
HX711 tensionCompressionScale;
HX711 torsionScale;


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

#define TORSION_LOADCELL_DOUT_PIN 2 // This is for torsion load cell
#define TORSION_LOADCELL_SCK_PIN 3
#define TC_LOADCELL_DOUT_PIN 4 // This is for tension/compression load cell
#define TC_LOADCELL_SCK_PIN 5
// AS5600 uses the I2C configuration i.e the SDA(A4), SCL(A5), Vcc and GND

#define VERNIER_CLOCK_PIN 9
#define VERNIER_DATA_PIN 8

// Test types sent from main page
enum TestType {
  TENSION,
  COMPRESSION,
  TORSION,
  NONE
};

float readCaliper();

// calibration factors
float tc_calibration_factor = -22.9926536;
float torsion_calibration_factor = -7050; //-20.0
TestType currentTest = NONE;
float initial_angle = 0.0;
long zero_factor;
char buf[20];
unsigned long tmpTime;
String serialInput = ""; // replaces String tare in the previous code used in handleSerialCommands function
// String tare;
int sign;
int inches;
long value;
float result;
bool mm = true;                //define mm to false if you want inches values
unsigned long timeout = 1000;  // Timeout in microseconds
bool s_load_cell_connected = false;
char outputBuffer[10];  // Buffer to hold the final string

//Function declarations

float readForce();
float readAngle(); // Called in the void setup 
void handleSerialCommands();
void setupLoadCells(); // Called in the void setup
void setupAS5600(); // Called in the void setup


void setupAS5600()
{
      // Check if AS5600 is connected
    Wire.beginTransmission(AS5600_ADDRESS);
    if (Wire.endTransmission() != 0) {
        Serial.println("AS5600 not found!");
        return;
    }
    Serial.println("AS5600 initialized");
}

void setupLoadCells() 
{
    // Initialize tension/compression load cell
    tensionCompressionScale.begin(TC_LOADCELL_DOUT_PIN, TC_LOADCELL_SCK_PIN);
    tensionCompressionScale.set_offset(4294962314);
    tensionCompressionScale.set_scale(tc_calibration_factor);
    tensionCompressionScale.tare();
    zero_factor = tensionCompressionScale.read_average();

    // Initialize torsion load cell
    torsionScale.begin(TORSION_LOADCELL_DOUT_PIN, TORSION_LOADCELL_SCK_PIN);
    torsionScale.set_scale(torsion_calibration_factor);
    torsionScale.tare();
    zero_factor = torsionScale.read_average();
}

float readAngle()
{
    Wire.beginTransmission(AS5600_ADDRESS);
    Wire.write(RAW_ANGLE_REGISTER);
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDRESS, 2);

    if (Wire.available() <= 2) {
        int highByte = Wire.read();
        int lowByte = Wire.read();
        int rawAngle = (highByte << 8) | lowByte;
        
        // Convert raw value to degrees (AS5600 has 12-bit resolution)
        float degrees = (rawAngle * 360.0) / 4096.0;
        
        // Calculate relative angle from initial position
        float relativeAngle = degrees - initial_angle;
        
        // Normalize to -180 to +180 range
        if (relativeAngle > 180) relativeAngle -= 360;
        if (relativeAngle < -180) relativeAngle += 360;
        
        return relativeAngle;
    }
    return 0;
}

void handleSerialCommands() {
    if (mySerial.available()) {
        serialInput = mySerial.readString();
        serialInput.trim();
        
        if (serialInput == "taring") {
            if (currentTest == TORSION) {
                torsionScale.tare();
                initial_angle = readAngle(); // Reset angle reference
            } else {
                tensionCompressionScale.tare();
            }
            Serial.println("Tared");
        }
        else if (serialInput == "tension") {
            currentTest = TENSION;
            Serial.println("Selected: Tension");
        }
        else if (serialInput == "compression") {
            currentTest = COMPRESSION;
            Serial.println("Selected: Compression");
        }
        else if (serialInput == "torsion") {
            currentTest = TORSION;
            initial_angle = readAngle(); // Reset angle reference when switching to torsion
            Serial.println("Selected: Torsion");
        }
    }
}

// Called in the void loop
float readForce() {
    float weight = 0.0;
    
    if (currentTest == TORSION) {
        weight = torsionScale.get_units(1);
    } else {
        weight = tensionCompressionScale.get_units(1);
    }
    
    // Round to one decimal place
    weight = floor(weight * 10) / 10;
    
    // Convert to Newtons and apply direction
    float forceInNewtons = (weight / 1000) * 9.81;
    
    if (currentTest == COMPRESSION) {
        forceInNewtons *= -1;
    }
    
    // Clean up near-zero values
    if (abs(forceInNewtons) < 0.15) {
        forceInNewtons = 0.0;
    }
    
    return forceInNewtons;
}

void setup() {
    Wire.begin();  // Initialize I2C
    mySerial.begin(4800);
    Serial.begin(115200);

    setupLoadCells();
    setupAS5600();
    
    pinMode(VERNIER_CLOCK_PIN, INPUT);
    pinMode(VERNIER_DATA_PIN, INPUT);

    // Store initial angle as reference
    initial_angle = readAngle();
}

// char TORSION[] = "torsion";
// char COMPRESSION[] = "compression";
// char TENSION[] = "tension";
// char RESET[] = "reset";
// String SELECTED;

void loop() 
{
  handleSerialCommands();
    
  if (currentTest != NONE) 
  {
    float force = readForce();
    float displacement = readCaliper();
    float angle = readAngle();
        
    // Send measurements in format: force,displacement,angle
    mySerial.print(force);
    mySerial.print(",");
    mySerial.print(displacement);
    mySerial.print(",");
    mySerial.print(angle);
    mySerial.println(); // End the message with a newline
        
    // Debug output
    Serial.print("Force (N): ");
    Serial.print(force);
    Serial.print(" Displacement (mm): ");
    Serial.print(displacement);
    Serial.print(" Angle (deg): ");
    Serial.println(angle);
  }
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
