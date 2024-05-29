#include <Servo.h>
#include <SoftwareSerial.h>

#define LEDPIN 13
#define SENSORPIN1 5
#define SENSORPIN2 6
#define SERVO_PIN 8

Servo servoMotor; 
SoftwareSerial bluetoothSerial(0, 1); 

int sensorState1 = 0;
int sensorState2 = 0;

String bluetoothInput = "";
String usbInput = "";

bool isOpen = false; 
unsigned long openTime = 0; 
const unsigned long openDuration = 3000;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(SENSORPIN1, INPUT);
  pinMode(SENSORPIN2, INPUT);
  digitalWrite(SENSORPIN1, HIGH); 
  digitalWrite(SENSORPIN2, HIGH); 

  servoMotor.attach(SERVO_PIN); 
  servoMotor.write(0); 

  Serial.begin(9600);
  bluetoothSerial.begin(9600); 
}

void loop() {
  sensorState1 = digitalRead(SENSORPIN1);
  sensorState2 = digitalRead(SENSORPIN2);

  String receivedString = ""; 

  while (bluetoothSerial.available() > 0) {
    char c = bluetoothSerial.read(); 

    if (c == '\n') {
      Serial.println(receivedString); 
      
      receivedString = ""; 
    } 
    else {
      receivedString += c;
    }
  }

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      if (usbInput == "1") { 
        Serial.println("Command received: 1");
        Serial.println("Deschis");
        servoMotor.write(90);
        isOpen = true;
        openTime = millis();
      }
      usbInput = "";
    } else {
      usbInput += c;
    }
  }


  if (isOpen && millis() - openTime >= openDuration) {

    sensorState1 = digitalRead(SENSORPIN1);

    sensorState2 = digitalRead(SENSORPIN2);


    if (sensorState1 && sensorState2) {

      Serial.println("Inchis");
      servoMotor.write(0);
      isOpen = false;
    } else {

      openTime = millis();
    }
  }

  delay(100); 
}
