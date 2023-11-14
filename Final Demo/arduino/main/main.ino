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


// Include libraries
#include <SpeedyStepper.h>

// Define NEMA 17 Motor Constants and Pins 
const int LED_PIN = 13;
const int BASE_A_STEP_PIN = 14;
const int BASE_A_DIR_PIN = 15;
const int BASE_B_STEP_PIN = 16;
const int BASE_B_DIR_PIN = 17;
const int THROW_STEP_PIN = 18;
const int THROW_DIR_PIN = 19;

// Initialise instances of Flexystepper class for each NEMA 17
//    Throw Arm stepper controls the ball throwing linkage structure
//    The base steppers A and B work in unison to either rotate or tilt the base
SpeedyStepper baseAStepper;
SpeedyStepper baseBStepper;
SpeedyStepper throwStepper;

void setup() {
      // Configure LED
    pinMode(LED_PIN, OUTPUT); 

    // start Serial comms
    Serial.begin(9600);

  // connect and configure the stepper motors to their IO pins
  baseAStepper.connectToPins(BASE_A_STEP_PIN, BASE_A_DIR_PIN);
  baseBStepper.connectToPins(BASE_B_STEP_PIN, BASE_B_DIR_PIN);
  throwStepper.connectToPins(THROW_STEP_PIN, THROW_DIR_PIN);

}

void loop() {
  // Check at least 4 chars in Serial queue
  if (Serial.available() > 0) {
    // read char to understand requested motion
    char movement = Serial.read();
    if (movement >= '1' && movement <= '5') {
          switch (movement) {
            case '1':
              // Case 1 corresponds to Very Underconfident shoot.
              behaviourVeryUnderconfident();
              break;
            case '2':
              // Case 2 corresponds to Underconfident shoot.
              behaviourUnderconfident();
              break;
            case '3':
              // Case 3 corresponds to Neutral shoot.
              behaviourNeutral();
              break;
            case '4':
              // Case 4 corresponds to Confident shoot.
              behaviourConfident();
              break;
            case '5':
              // Case 5 corresponds to Very Confident shoot.
              behaviourVeryConfident();
              break;
          }
        } else {
          blinkLED(1);
          return;
        }
      }
}

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);  // Turn the LED on
    delay(500);                 // Wait for 500 milliseconds (0.5 seconds)
    digitalWrite(LED_PIN, LOW);   // Turn the LED off
    delay(500);                 // Wait for 500 milliseconds
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
  delay(250);
  moveXYWithCoordination(-800, -800, 1200, 1200);
  delay(500);
  throwBall(1400, 45);
  delay(500);
  moveXYWithCoordination(800, 800, 1200, 1200);

  // After throw, return to neutral position
  Serial.println("Very underconfident action. \n");

}

void behaviourUnderconfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray an unconfident moment.
  // Movement: Quivering Tilt and Aim. Tilt finishes off target. Rotation on target.
  // Ball: Throw and miss
  delay(250);
  moveXYWithCoordination(200, 200, 1600, 1600);
  delay(250);
  moveXYWithCoordination(-200, -200, 1600, 1600);
  delay(500);
  moveXYWithCoordination(200, 200, 1600, 1600);
  delay(250);
  moveXYWithCoordination(-200, -200, 1600, 1600);
  delay(500);
  moveXYWithCoordination(200, 200, 1600, 1600);
  delay(250);
  moveXYWithCoordination(-200, -200, 1600, 1600);
  delay(500);
  throwBall(1200, 45);

  // After throw, return to neutral position
  Serial.println("Underconfident action. \n");

}

void behaviourNeutral(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a neutral moment.
  // Movement: Quivering Tilt and Aim. Tilt and Rotation finishes on target.
  // Ball: Throw and land
  delay(500);
  moveXYWithCoordination(150, 150, 1600, 1600);
  delay(500);
  moveXYWithCoordination(-150, -150, 1600, 1600);
  delay(500);
  moveXYWithCoordination(-150, -150, 1600, 1600);
  delay(500);
  moveXYWithCoordination(150, 150, 1600, 1600);
  delay(500);
  throwBall(1800, 42);

}

void behaviourConfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a confident moment.
  // Movement: Clear, concise movement in Tilt and Aim. Tilt and Rotation finishes on target. 
  // Ball: Throw and land
  delay(500);
  throwBall(1800, 45);

}

void behaviourVeryConfident(){
  // This function controls the stepper motors to play out a specific animation
  // This animation should portray a very confident moment.
  // Movement: Trick shot. 180 rotation left, 180 rotation right, Throw high into the hoop. 
  // Ball: Throw and land
    delay(500);
    moveXYWithCoordination(800, 800, 1600,1600);
    delay(500);
    moveXYWithCoordination(-800, -800, 1600, 1600);
    delay(500);
    throwBall(1800, 45);


  // After throw, return to neutral position
  Serial.println("Very confident action. \n");

}

// rotate base
void moveXYWithCoordination(long stepsX, long stepsY, float speedInStepsPerSecond, float accelerationInStepsPerSecondPerSecond)
{
  float speedInStepsPerSecond_X;
  float accelerationInStepsPerSecondPerSecond_X;
  float speedInStepsPerSecond_Y;
  float accelerationInStepsPerSecondPerSecond_Y;
  long absStepsX;
  long absStepsY;

  // setup initial speed and acceleration values
  speedInStepsPerSecond_X = speedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_X = accelerationInStepsPerSecondPerSecond;
  
  speedInStepsPerSecond_Y = speedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_Y = accelerationInStepsPerSecondPerSecond;


  //
  // determine how many steps each motor is moving
  //
  if (stepsX >= 0)
    absStepsX = stepsX;
  else
    absStepsX = -stepsX;
 
  if (stepsY >= 0)
    absStepsY = stepsY;
  else
    absStepsY = -stepsY;


  //
  // determine which motor is traveling the farthest, then slow down the
  // speed rates for the motor moving the shortest distance
  //
  if ((absStepsX > absStepsY) && (stepsX != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsY / (float) absStepsX;
    speedInStepsPerSecond_Y = speedInStepsPerSecond_Y * scaler;
    accelerationInStepsPerSecondPerSecond_Y = accelerationInStepsPerSecondPerSecond_Y * scaler;
  }
  
  if ((absStepsY > absStepsX) && (stepsY != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsX / (float) absStepsY;
    speedInStepsPerSecond_X = speedInStepsPerSecond_X * scaler;
    accelerationInStepsPerSecondPerSecond_X = accelerationInStepsPerSecondPerSecond_X * scaler;
  }

  
  //
  // setup the motion for the X motor
  //
  baseAStepper.setSpeedInStepsPerSecond(speedInStepsPerSecond_X);
  baseAStepper.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_X);
  baseAStepper.setupRelativeMoveInSteps(stepsX);


  // setup the motion for the Y motor
  baseBStepper.setSpeedInStepsPerSecond(speedInStepsPerSecond_Y);
  baseBStepper.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_Y);
  baseBStepper.setupRelativeMoveInSteps(stepsY);

  // now execute the moves, looping until both motors have finished
  while((!baseAStepper.motionComplete()) || (!baseBStepper.motionComplete()))
  {
    baseAStepper.processMovement();
    baseBStepper.processMovement();
  }
}

void throwBall(int power, int steps) {
  throwStepper.setSpeedInStepsPerSecond(power);
  throwStepper.setAccelerationInStepsPerSecondPerSecond(power);
  throwStepper.moveRelativeInSteps(-steps);
  delay(500);
  throwStepper.moveRelativeInSteps(steps);
  delay(500);
}