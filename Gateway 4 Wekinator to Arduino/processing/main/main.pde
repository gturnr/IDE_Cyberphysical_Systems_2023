import processing.serial.*;
import oscP5.*;

// define Serial and OSC object
Serial port;
OscP5 oscP5;  
color backgroundColor = color(0);  
color[] backgroundColors = {color(255, 0, 0), color(0, 255, 0), color(0, 0, 255)};
int previousInput = 0;

void setup() {
  size(100, 100);
  println(Serial.list());
  String portName = Serial.list()[1];
  port = new Serial(this, portName, 9600);

  oscP5 = new OscP5(this, 12000);

}

void draw() {
  fill(backgroundColor, 10);
  textAlign(CENTER, CENTER);
  textSize(20);
  fill(255); // White text color
  
  // Title
  text("Passthrough", width/2, 40);
}

void oscEvent(OscMessage theOscMessage) {  
  int oscArg = int(theOscMessage.get(0).floatValue());
  println(oscArg);
  
  // check if input has changed since last to save excessive serial messages
  if (oscArg != previousInput) {
    previousInput = oscArg;
    backgroundColor = backgroundColors[oscArg-1];

    if (oscArg == 1) {
      port.write("R1");
      port.write("Y0");
      port.write("G0");
    } else if (oscArg == 2) {
      port.write("R0");
      port.write("Y1");
      port.write("G0");
    } else if (oscArg == 3) {
      port.write("R0");
      port.write("Y0");
      port.write("G1");
    } else {
      println("Input is not 1, 2, or 3");
    }
  }
}
