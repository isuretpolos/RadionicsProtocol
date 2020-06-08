String commands [] = {"commands", "analyze", "broadcast", "broadcast pattern", "reset", "grounding", "trng", "clear"};
String readString;
const int ledPin =  LED_BUILTIN;
const int coilPin =  12;
String signature;
int lastCommand;

void setup() {
  Serial.begin(9600);
  Serial.println("AETHERONE");
  pinMode(ledPin, OUTPUT);
  pinMode(coilPin, OUTPUT);
  digitalWrite(coilPin, HIGH);
  delay(200);
  digitalWrite(coilPin, LOW);
}

void loop() {

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();

    if (c == 10) {
      interpretCommand();
      digitalWrite(coilPin, LOW);
    } else {
      digitalWrite(coilPin, HIGH);
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

void clearProcess() {
  signature = "";
  lastCommand = NULL;
}

void broadcast() {
  
  if (!signature) {
    lastCommand = NULL;
  }

  for (int i=0; i<signature.length(); i++) {
    int c = signature.charAt(i);
    
    if ((c & 1) == 0) {
      digitalWrite(coilPin, HIGH);
    } else {
      digitalWrite(coilPin, LOW);
    }

    delay(5);
  }
}

void interpretCommand() {

  //if (readString.length() == 0) return;
  digitalWrite(coilPin, HIGH);
  if (readString.length() > 2) {
    digitalWrite(coilPin, HIGH);
    Serial.println("- - [" + readString.substring(2) + "]");
    int command = readString.substring(0, 1).toInt();
    Serial.println(command);
    Serial.println("this is the command I received " + commands[command]);
    lastCommand = command;
    if (command == 2) {
      signature = readString.substring(2);
    }
    digitalWrite(coilPin, LOW);
  } else {
    Serial.println("- " + readString);
  }

  delay(200);
  digitalWrite(coilPin, LOW);

  readString = "";
}
