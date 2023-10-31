// Title:       main.pde
//
// Date:        26/10/2023
//
// Description: Processing code to collect OSC messages from audio sentiment model, and 
//              process the average before sending to Arduino over serial.
//
// Purpose:     Written in part for final project submission to Cyber physical Systems 
//              module 2023 IDE 1 Imperial College London and Royal College of Art. 
//
// Author(s):   Guy Turner glt23@ic.ac.uk
// License:     AFL-3.0

// import libraries used for Serial and OSC comms
import processing.serial.*;
import oscP5.*;

// define instances of the Serial and OscP5 classes
Serial port;
OscP5 oscP5;  

// initialise global variables used throughout
char[] movementIndex = {'A', 'B', 'C', 'D', 'E'}; // Behaviours that can be sent to Arduino
int lastReceivedOSC = -1;
int lastOSC = -1;
int[] sentiment = new int[100]; // Initialize an empty array to store integers
int sentimentIndex = 0;
int running = 0;
int startTimer = -1;
String[] buttonLabels = new String[3];

// Function: setup. Purpose: configure GUI, intialise OSC and Serial comms
void setup() {
  // create window 400 W x 600 H pixels
  size(400, 600);
  
  // define button labels
  buttonLabels[0] = "Start Capture";
  buttonLabels[1] = "Running";
  buttonLabels[2] = "Reset";
  
  // scan for all Serial ports and define Arduino Port as index 1 in list
  println(Serial.list());
  String portName = Serial.list()[1];
  
  // initialise Serial Port and OSC connection
  port = new Serial(this, portName, 9600);
  oscP5 = new OscP5(this, 12000);
}

// Function: draw. Purpose: draw GUI each new frame. Render text, boxes, and call relevant functionality
void draw() {
  // Set background colour to white and render over previous frame
  background(255); 
  
  // Title
  textAlign(CENTER, CENTER); textSize(20); fill(0);
  text("Emotional Basketball Arm ", width/2, 25);
  text("Sentiment Analysis", width/2, 50);
  
  // OSC Input indicator
  textAlign(LEFT, CENTER);textSize(12); fill(0); text("Receiving OSC", 55, 85);
  // check if an OSC msg has been received in the last 100ms 
  if ((lastReceivedOSC + 100) > millis()){
    // show green circle to demonstrate incoming OSC msgs
    fill(0,255,0); stroke(0);circle(45, 85, 10);
  } else {
    // show red circle to demonstrate no incoming OSC msgs
    fill(255,0,0); stroke(0);circle(45, 85, 10);
  }
    
  // Start Button and instructions
  rectMode(CENTER); fill(255); rect(200, 140, 320, 40);
  fill(0); textAlign(CENTER, CENTER); textSize(30); text(buttonLabels[running], 200, 140);
  textAlign(CENTER, CENTER);textSize(16); fill(0); text("Press Start to collect 10 seconds of sentiment.", 200, 180);
  text("Load ball first.", 200, 200);

  // Check if sentiment collection is currently running. If so, run the processSentiment function this framer
  if (running == 1){
    processSentiment();
  }

  // Redner the latest version of the sentiment collection graph
  drawGraph(300, 250, 60, 300, 100);
}

// Function: processSentiment. Description: Sample OSC messages and then calculate sentiment average
void processSentiment() {
  // if Sentiment analysis is running and it is within 10 seconds since starting
  if (millis() < (startTimer + 10 * 1000)){
    // then we are sampling. Take a sample every 100ms and add to sentiment. 
    if (millis() > (startTimer + sentimentIndex * 100)) {
      println("Taking sample " + sentimentIndex);
      // append latest sample to sentiment array
      sentiment[sentimentIndex] = lastOSC;
      // increase number of sentiments taken by 1
      sentimentIndex += 1;
    }
  } else {
    // sampling over. Work out the average sentiment, send to Arduino.
    // sum up all the sentiment values and avg
    int sum = 0;
    for (int i = 0; i < 100; i++) {
      sum += sentiment[i];
    }
    
    int averageSentiment = round(float(sum) / 100);
    // send Serial message to Arduino 
    println("Final sentiment: " + averageSentiment);
    port.write(str(averageSentiment));
    
    // set program mode to 2, which gives user option to reset. 
    running = 2;
  }
}

// Function: drawGraph. Purpose: to render a bar graph of all OSC samples taken so far
void drawGraph(int w, int h, int x, int y, int n) {
  // calculate pixel width of each bar graph column
  int columnWidth = w / n;
  // iterate through each bar graph column
  for (int i = 0; i < n; i++) {
    // calculate the height for the column based on the OSC message value. Range of 1 to 5
    int columnHeight = int(map(sentiment[i], 0, 5, 0, h));
    // choose the colour for the bar graph column based on value. 
    // Maps as 1: red, 2: oranage, 3: yellow, 4: light green, 5: deep green
    int col;
    switch (sentiment[i]) {
      case 1:
        col = color(255, 0, 0);
        break;
      case 2:
        col = color(255, 193, 0);
        break;
      case 3:
        col = color(255, 255, 0);
        break;
      case 4:
        col = color(214, 255, 0);
        break;
      case 5:
        col = color(92, 255, 0);
        break;
      default:
        col = color(0);
        break;
    }
    // draw the column rectangle in the right x,y position
    fill(col);
    rectMode(CORNER);
    rect(x + (i * columnWidth), y + h - columnHeight, columnWidth, columnHeight);
  }
}

void mousePressed() {
   // Check if start button has been clicked. This is done by checking if the mouse cursor
   // is within the x,y bounds of the rectangle
   if (mouseX > 40  && mouseX < (width - 40) && mouseY > 120 && mouseY < 160) {
      // based on the current running state, change the program functionality
      if (running == 0){
        // if not running, start sample collection
        running = 1;
        startTimer = millis();
        println("Starting capture for 10 seconds.");
      } else if (running == 1){
        // if running sampling but not finished, stop sampling prematurely
        println("Stopping.");
        running = 2;
      } else if (running == 2){
        // if sampling over, allow user to clear graph and stored values to start collection again
        println("Resetting to collect sentiment again.");
        // empty all the vars that contain values from the last run
        running = 0;
        startTimer = -1;
        sentiment = new int[100];
        sentimentIndex = 0;
      }
   }
}

// Summary: Handles incoming OSC messages.
void oscEvent(OscMessage theOscMessage) {  
  int oscArg = int(theOscMessage.get(0).floatValue());
  lastReceivedOSC = millis();
  lastOSC = oscArg;
}
