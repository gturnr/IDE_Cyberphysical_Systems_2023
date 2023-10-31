#include <SpeedyStepper.h>


//
// pin assignments
//
const int LED_PIN = 13;
const int BASE_A_STEP_PIN = 14;
const int BASE_A_DIR_PIN = 15;
const int BASE_B_STEP_PIN = 16;
const int BASE_B_DIR_PIN = 17;
const int THROW_STEP_PIN = 18;
const int THROW_DIR_PIN = 19;


//
// create two stepper motor objects, one for each motor
//
SpeedyStepper baseAStepper;
SpeedyStepper baseBStepper;
SpeedyStepper throwStepper;



void setup() 
{
  // setup the LED pin and enable print statements
  pinMode(LED_PIN, OUTPUT);   
  Serial.begin(9600);

  // connect and configure the stepper motors to their IO pins
  baseAStepper.connectToPins(BASE_A_STEP_PIN, BASE_A_DIR_PIN);
  baseBStepper.connectToPins(BASE_B_STEP_PIN, BASE_B_DIR_PIN);
  throwStepper.connectToPins(THROW_STEP_PIN, THROW_DIR_PIN);
}


void loop() 
{

  float speed = 1200;
  float acc = 1200;
  //moveXYWithCoordination(800, 800, speed, acc);
  //delay(1000);
  delay(500);
  moveXYWithCoordination(400, 400, 1200, 1200);
  delay(500);
  moveXYWithCoordination(-800, -800, 1200, 1200);
  delay(500);
  moveXYWithCoordination(900, 900, 1800, 1800);
  delay(500);
  moveXYWithCoordination(-450, -450, 1800, 1800);
  delay(1000);
  
  // 2600 middle, 3000 pitched back
  throwStepper.setSpeedInStepsPerSecond(3000);
  throwStepper.setAccelerationInStepsPerSecondPerSecond(3000);
  throwStepper.moveRelativeInSteps(-55);
  delay(500);
  throwStepper.moveRelativeInSteps(55);
  delay(500);
}

// move both X & Y motors together in a coordinated way, such that they each 
// start and stop at the same time, even if one motor moves a greater distance
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

