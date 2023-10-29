// Cyberphysical Systems 2023 
// Group 8 Submission
// Author(s): Guy Turner glt23@ic.ac.uk
// License: AFL-3.0

// Define constants for LED pins
const int redPin = 17;
const int yellowPin = 16;
const int greenPin = 15;

void setup() {
    // start Serial 
    Serial.begin(9600);
    
    // initialise LED pins
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    digitalWrite(redPin, 1);
    digitalWrite(yellowPin, 1);
    digitalWrite(greenPin, 1);
}

void loop() {
  // Check at least 2 chars in Serial queue
  if (Serial.available() >= 2) {
    // read first char which contains LED identifier (R,Y,G)
    char ledName = Serial.read();
    if (ledName == 'R' || ledName == 'Y' || ledName == 'G') {
        // read second char which is power on/off 1/0
        char powerChar = Serial.read();
        if (powerChar == '1' || powerChar =='0') {
          int power = 0;
          switch (powerChar) {
            case '1':
              power = 1;
              break;
            case '0':
              power = 0;
              break;
          };
      
          switch (ledName) {
            case 'R':
              digitalWrite(redPin, power);
              break;
            case 'Y':
              digitalWrite(yellowPin, power);
              break;
            case 'G':
              digitalWrite(greenPin, power);
              break;
          }
          Serial.println("LED " + String(ledName) + " set to: " + String(power));

        } else {
          Serial.println("Error: Invalid power. Either 1 or 0.");
          return;
        }
      }
  }
}
