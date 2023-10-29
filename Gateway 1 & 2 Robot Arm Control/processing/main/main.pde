import processing.serial.*;

Serial port;

// define arrays
int[] rowValues = new int[4]; // Array to store integer values for each row
char[] servoIndex = {'A', 'B', 'C', 'D'}; // Servo labels for serial comms
String[] rowLabels = new String[4]; // Strings for row labels

void setup() {
  size(400, 600);
  println(Serial.list());
  String portName = Serial.list()[1];
  port = new Serial(this, portName, 9600);
  
  // Assign values to rowLabels
  rowLabels[0] = "Base Servo";
  rowLabels[1] = "X-Axis Servo";
  rowLabels[2] = "Y-Axis Servo";
  rowLabels[3] = "Claw Servo";
}

void draw() {
  background(66, 135, 245); // Clear previous frame
  textAlign(CENTER, CENTER);
  textSize(20);
  fill(255); // White text color
  
  // Title
  text("Robot Arm Control - Group 8", width/2, 40);

  // Display the updated values and buttons
  for (int i = 0; i < 4; i++) {
    int yPos = 120 + i * 100;
    
    // Left button with a minus sign
    rectMode(CENTER);
    fill(255);
    rect(100, yPos, 40, 40);
    fill(0);
    textSize(30);
    text("-", 100, yPos);
    
    // Right button with a plus sign
    fill(255);
    rect(300, yPos, 40, 40);
    fill(0);
    textSize(30);
    text("+", 300, yPos);
    
    // Text in the middle for the integer
    fill(0);
    textSize(50);
    text(nf(rowValues[i], 3), width/2, yPos); // Display the value from the array
    
    // Text title in the top left corner
    textSize(16);
    text(rowLabels[i], 60, yPos - 40);
  }
}

void mousePressed() {
  for (int i = 0; i < 4; i++) {
    int yPos = 120 + i * 100;
    
    // Check if the minus button is clicked
    if (mouseX > 80 - 25 && mouseX < 80 + 25 && mouseY > yPos - 25 && mouseY < yPos + 25) {
      // Deduct 10 from the value (not below 0)
      rowValues[i] = max(0, rowValues[i] - 10);
      println("Row " + (i+1) + ": Minus clicked. Value: " + rowValues[i]);
      println(servoIndex[i] + nf(rowValues[i], 3));
      port.write(servoIndex[i] + nf(rowValues[i], 3));
    }
    
    // Check if the plus button is clicked
    if (mouseX > 300 - 25 && mouseX < 300 + 25 && mouseY > yPos - 25 && mouseY < yPos + 25) {
      // Add 10 to the value (not above 180)
      rowValues[i] = min(180, rowValues[i] + 10);
      println("Row " + (i+1) + ": Plus clicked. Value: " + rowValues[i]);
      println(servoIndex[i] + nf(rowValues[i], 3));
      port.write(servoIndex[i] + nf(rowValues[i], 3));
    }
  }
}
