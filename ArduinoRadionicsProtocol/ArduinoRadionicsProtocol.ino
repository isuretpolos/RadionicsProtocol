/**
 * Copyright Isuret Polos 2020
 * "Remedy Maker", or better a radionics service device which does what it is intented for.
 */

// --- Begin LED definition
const int COILpin =  12;
const int REDpin =  11;
const int GREENpin =  10;
const int BLUEpin =  9;
const int WHITEpin =  8;
const int ULTRAVIOLETpin = 7;
const int SIGNAL_DURATION = 5;
// --- End LED definition

String commands [] = {"commands", "analyze", "broadcast", "broadcast pattern", "reset", "grounding", "trng", "clear"};
String readString;
String signature;
int lastCommand;

void setup() {
  Serial.begin(9600);
  Serial.println("AETHERONE");
  
  pinMode(COILpin, OUTPUT);
  pinMode(REDpin, OUTPUT);
  pinMode(GREENpin, OUTPUT);
  pinMode(BLUEpin, OUTPUT);
  pinMode(WHITEpin, OUTPUT);
  pinMode(ULTRAVIOLETpin, OUTPUT);
  digitalWrite(COILpin, HIGH);
  delay(200);
  digitalWrite(COILpin, LOW);
}

/**
 * Main Loop
 */
void loop() {

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();

    if (c == 10) {
      interpretCommand();
      digitalWrite(COILpin, LOW);
    } else {
      digitalWrite(COILpin, HIGH);
      readString += c;
    }
  }

  if (lastCommand == 2) {
    broadcast();
  }

  if (lastCommand == 7) {
    clearProcess();
  }
}

/**
 * Insert here the process of clearing your device
 * and everything which need a reset for the next session
 */
void clearProcess() {
  signature = "";
  lastCommand = NULL;
}

/**
 * Broadcast the signature
 */
void broadcast() {
  
  if (!signature) {
    lastCommand = NULL;
  }

  for (int i=0; i<signature.length(); i++) {
    int c = signature.charAt(i);

    // let the aether vibrate
    if ((c & 1) == 0) {
      digitalWrite(COILpin, HIGH);
    } else {
      digitalWrite(COILpin, LOW);
    }

    sendLightSignal(c);

  }
}

/**
 * Taking appart the singel digits and assign them to the color(ed) LEDs.
 * And YES, you can replace some of them with LASER diodes (be careful with your eyes).
 */
void sendLightSignal(int c) {
  while (c > 0) {
        
        int digit = c%10;
        c /= 10;

        // red is the base (chakra)
        if (digit == 0 || digit == 1 || digit == 2) {
          sendSignal(REDpin);
        }

        // green is the heart (center/love)
        if (digit == 5 || digit == 7) {
          sendSignal(GREENpin);
        }

        // blue is order and structure (blueprint)
        if (digit == 4 || digit == 8) {
          sendSignal(BLUEpin);
        }

        // 3,6 and 9 is the vortex (Marko Rodin)
        if (digit == 3 || digit == 6 || digit == 9) {
          sendSignal(WHITEpin);
        }
   }
}

/**
 * Send a signal for a pin with predifenite duration
 */
void sendSignal(int pin) {
  sendSignal(pin, SIGNAL_DURATION);
}

/**
 * Send a signal for a pin with a specific duration in milliseconds
 */
void sendSignal(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

/**
 * Interpret the command received from the serial communication
 */
void interpretCommand() {

  //if (readString.length() == 0) return;
  digitalWrite(COILpin, HIGH);
  if (readString.length() > 2) {
    digitalWrite(COILpin, HIGH);
    Serial.println("- - [" + readString.substring(2) + "]");
    int command = readString.substring(0, 1).toInt();
    Serial.println(command);
    Serial.println("this is the command I received " + commands[command]);
    lastCommand = command;
    if (command == 2) {
      signature = readString.substring(2);
    }
    digitalWrite(COILpin, LOW);
  } else {
    Serial.println("- " + readString);
  }

  delay(200);
  digitalWrite(COILpin, LOW);

  readString = "";
}
