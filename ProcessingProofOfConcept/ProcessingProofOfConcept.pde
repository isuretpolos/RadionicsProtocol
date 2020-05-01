import processing.serial.*;

Serial myPort;
String text;

void setup() 
{
  size(200, 200);

  String preferredortName = "COM3"; // preferred port name
  
  for (String usbPortName : Serial.list()) {
    println(usbPortName);
  }
  
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  background(255);   
  if ( myPort.available() > 0) {  // If data is available,
    fill(0,255,0);          // read it and store it in val
    text = myPort.readString();
  } else {
    fill(0);
  }

  if (text != null)  text(text, 10, 10);
  rect(50, 50, 100, 100);
}



/*

// Wiring / Arduino Code
// Code for sensing a switch status and writing the value to the serial port.

int switchPin = 4;                       // Switch connected to pin 4

void setup() {
  pinMode(switchPin, INPUT);             // Set pin 0 as an input
  Serial.begin(9600);                    // Start serial communication at 9600 bps
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {  // If switch is ON,
    Serial.write(1);               // send 1 to Processing
  } else {                               // If the switch is not ON,
    Serial.write(0);               // send 0 to Processing
  }
  delay(100);                            // Wait 100 milliseconds
}

*/
