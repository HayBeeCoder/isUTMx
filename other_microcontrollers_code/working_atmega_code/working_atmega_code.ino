#include <SoftwareSerial.h>
#define LEDPIN_1 9
#define LEDPIN_2 10
//L298N controller pins
#define IN1 5
#define IN2 6
#define ENA 3  // PWM pin to control motor speed
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

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  digitalWrite(LEDPIN_1,LOW);
  digitalWrite(LEDPIN_2,LOW); 

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.begin(115200);
  mySerial.begin(4800);
}

void loop() {
  check_mySerial_data();

  // Handle motor control when the received value is 21 (TORSION_START)
  if (value_from_esp == TORSION_START) {
    // Set motor direction
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    analogWrite(ENA, 255);  // 255 = full speed,i will adjust as necessary
    
    digitalWrite(LEDPIN_1, HIGH);
    digitalWrite(LEDPIN_2, LOW);

  } else if (value_from_esp == TORSION_STOP) {
    analogWrite(ENA, 0);  // Set motor speed to 0 to stop it
    digitalWrite(LEDPIN_1, LOW);
    digitalWrite(LEDPIN_2, HIGH);

  } 
  else if(value_from_esp == TENSION_OR_COMPRESSION_START){
    digitalWrite(LEDPIN_1, HIGH);
    digitalWrite(LEDPIN_2, LOW);

  }else if(value_from_esp == TENSION_OR_COMPRESSON_STOP){
    digitalWrite(LEDPIN_1, LOW);
    digitalWrite(LEDPIN_2,HIGH);
  }
  // digitalWrite(LEDPIN_1,HIGH);
  // digitalWrite(LEDPIN_2,HIGH);
  delay(1000);
}


// https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/