// Title:       main.ino
//
// Date:        26/10/2023
//
// Description: Arduino Nano Every code to control 3 NEMA 17 stepper motors 
//              driven by Adafruit TB6612 drivers. Listens for communication over 
//              serial to control motion. 
//
// Purpose:     Written in part for final project submission to Cyber physical Systems 
//              module 2023 IDE 1 Imperial College London and Royal College of Art. 
//
// Author(s):   Guy Turner glt23@ic.ac.uk
// License:     AFL-3.0


// include libraries
#include <Stepper.h>

// Define number of steps for full rotation on NEMA 17
#define STEPS 200

#define ROTATION_LEFT_MAX_STEPS

// Initialise instances of Stepper class for each NEMA 17
//    Throw Arm stepper controls the ball throwing linkage structure
//    The base steppers A and B work in unison to either rotate or tilt the base
Stepper throwArmStepper(STEPS, 4, 5, 6, 7);
Stepper baseStepperA(STEPS, 4, 5, 6, 7);
Stepper baseStepperB(STEPS, 4, 5, 6, 7);

void setup() {
    // start Serial comms
    Serial.begin(9600);
}

void loop() {
  // Check at least 4 chars in Serial queue
  if (Serial.available()) {
    // read char to understand requested motion
    char movement = Serial.read();
    if (movement == 'A' || movement == 'B' || movement == 'C' || movement == 'D' || movement == 'E') {
          switch (movement) {
            case 'A':
              // Case A corresponds to Very Underconfident shoot.
              behaviourVeryUnderconfident();
              break;
            case 'B':
              // Case B corresponds to Underconfident shoot.
              behaviourUnderconfident();
              break;
            case 'C':
              // Case C corresponds to Neutral shoot.
              behaviourNeutral();
              break;
            case 'D':
              // Case D corresponds to Confident shoot.
              behaviourConfident();
              break;
            case 'E':
              // Case E corresponds to Very Confident shoot.
              behaviourVeryConfident();
              break;
          }
        } else {
          // Invalid character sent. Pass.
          return;
        }
      }
  }
}

// behaviour functions based on desired performance:
// 1. Very underconfident
// 2. Underconfident
// 3. Neutral
// 4. Confident
// 5. Very confident

void behaviourVeryUnderconfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a very unconfident moment.
  // Movement: Quivering Tilt and Aim. Tilt and Rotation finishes off target.
  // Ball: Throw and miss

  // After throw, return to neutral position

}

void behaviourUnderconfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray an unconfident moment.
  // Movement: Quivering Tilt and Aim. Tilt finishes off target. Rotation on target.
  // Ball: Throw and miss

  // After throw, return to neutral position

}

void behaviourNeutral(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a very unconfident moment.
  // Movement: Quivering Tilt and Aim. Tilt and Rotation finishes on target.
  // Ball: Throw and land

  // After throw, return to neutral position

}

void behaviourConfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a very unconfident moment.
  // Movement: Clear, concise movement in Tilt and Aim. Tilt and Rotation finishes on target. 
  // Ball: Throw and land

  // After throw, return to neutral position

}

void behaviourVeryConfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a very unconfident moment.
  // Movement: Trick shot. 180 rotation left, 180 rotation right, lean forward, back, Throw high into the hoop. 
  // Ball: Throw and land


  // After throw, return to neutral position

}