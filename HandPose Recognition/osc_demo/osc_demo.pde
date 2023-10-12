import oscP5.*;

OscP5 oscP5;  
color backgroundColor = color(0);  
color[] backgroundColors = {color(255, 0, 0), color(0, 255, 0), color(0, 0, 255)};

void setup() {  
  size(400, 400);
  oscP5 = new OscP5(this, 12000);
  noStroke();
}

void draw() {  
  fill(backgroundColor, 10);
  rect(0, 0, width, height);
}

void oscEvent(OscMessage theOscMessage) {  
  int oscArg = int(theOscMessage.get(0).floatValue());
  println(oscArg);
  backgroundColor = backgroundColors[oscArg-1];
}
