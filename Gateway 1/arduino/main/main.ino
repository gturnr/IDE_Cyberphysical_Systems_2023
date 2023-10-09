#include "PCA9685.h"
#include <Wire.h>

ServoDriver servo;

// Define constants for servo pins
const int baseServoPin = 6;
const int xServoPin = 5;
const int yServoPin = 7;
const int clawServoPin = 8;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(9600);
    servo.init(0x7f);

}

void loop() {
  if (Serial.available() >= 4) {
    char servoName = Serial.read();
    Serial.println(servoName);
    if (servoName == 'A' || servoName == 'B' || servoName == 'C' || servoName == 'D') {
      int angle = 0;
      for (int i = 0; i < 3; i++) {
        char digit = Serial.read();
        if (digit >= '0' && digit <= '9') {
          angle = angle * 10 + (digit - '0');
        } else {
          Serial.println("Error: Invalid character in angle.");
          return;
        }
      }
      if (angle >= 15 && angle <= 165) {
        switch (servoName) {
          case 'A':
            servo.setAngle(baseServoPin, angle);
            break;
          case 'B':
            servo.setAngle(xServoPin, angle);
            break;
          case 'C':
            servo.setAngle(yServoPin, angle);
            break;
          case 'D':
            servo.setAngle(clawServoPin, angle);
            break;
        }
        Serial.println("Servo " + String(servoName) + " set to angle: " + String(angle));
      } else {
        Serial.println("Error: Angle out of range (0-180).");
      }

    } else {
      Serial.println("Error: Invalid servo name.");
    }
  }
}
