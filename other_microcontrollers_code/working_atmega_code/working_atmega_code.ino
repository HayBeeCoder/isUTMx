#include <SoftwareSerial.h>
#define LEDPIN_1 9
#define LEDPIN_2 10

SoftwareSerial mySerial(12, 13);

String serialData = "";
String data;
int value_from_esp;


enum STATUS {
  TENSION_OR_COMPRESSION_START = 11,
  TENSION_OR_COMPRESSON_STOP = 10,
  TORSION_START = 21,
  TORSION_STOP = 20,
};


void check_mySerial_data() {
  // Check if there's data available on Serial2
  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read();  // Read one character at a time
    // Append the character to the serialData string until '\n' is received
    if (receivedChar != '\n') {
      serialData += receivedChar;
    } else {
      // Once a full line is received, process the data
      value_from_esp = serialData.toInt();

      serialData = "";  // Clear the buffer for the next data line
    }
  }
}

void setup() {
  // Initialize mySerial for communication with ESP32
  // mySerial.begin(9600);
  pinMode(LEDPIN_1, OUTPUT);
  pinMode(LEDPIN_2, OUTPUT);
  digitalWrite(LEDPIN_1,LOW);
  digitalWrite(LEDPIN_2,LOW);
  Serial.begin(115200);
  mySerial.begin(4800);
}

void loop() {
  check_mySerial_data();
  // Serial.println()
  if(value_from_esp == TENSION_OR_COMPRESSION_START){
  digitalWrite(LEDPIN_2,LOW);

  }else if(value_from_esp == TENSION_OR_COMPRESSON_STOP){
  digitalWrite(LEDPIN_2,HIGH);

  }
  digitalWrite(LEDPIN_1,HIGH);
  // digitalWrite(LEDPIN_2,HIGH);
  delay(1000);
}
