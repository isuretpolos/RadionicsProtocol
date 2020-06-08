import processing.serial.*;
import java.util.*;

Serial arduino;
int baud = 9600;
String text;
int lf = 10;
Map<String, Button> mapButtons = new HashMap<String, Button>();

void setup() {

  size(600, 400);

  for (String usbPortName : Serial.list()) {
    
    println(usbPortName);
    
    try {
      println("try to connect");
      arduino = new Serial(this, usbPortName, baud);
      if ( arduino.available() > 0) {
        String message = arduino.readStringUntil(lf);
        if ("AETHERONE".equals(message)) {
          println("connected with Arduino at port " + usbPortName + " and baud " + baud);
          break;
        }
      }
    } catch(Exception e) {
      e.printStackTrace();
    }
      
  }
  
  int yButtonDraw = 40;
  yButtonDraw = addButton("commands", yButtonDraw);
  yButtonDraw = addButton("analyze", yButtonDraw);
  yButtonDraw = addButton("broadcast", yButtonDraw);
  yButtonDraw = addButton("broadcast pattern", yButtonDraw);
  yButtonDraw = addButton("reset", yButtonDraw);
  yButtonDraw = addButton("grounding", yButtonDraw);
  yButtonDraw = addButton("trng", yButtonDraw);
  yButtonDraw = addButton("clear", yButtonDraw);
}

void draw() {

  background(255);
  fill(0);
  stroke(0); text("RADIONICS PROTOCOL TEST SUITE",10,10);

  if ( arduino.available() > 0) {
    String newText = arduino.readStringUntil(lf);
    if (newText != null) {
      text = newText;
      println(text);
    }
  }

  if (text != null)  text(text, 10, 20);

  text(mouseX + "," + mouseY, 200, 20);
  
  for (Button button : mapButtons.values()) {
    drawButton(button);
  }
}

void drawButton(Button button) {
  rect(10, button.y, 40, 40);
  text(button.command, 60, button.y + 25);
}

int addButton(String command, int y) {
  Button button = new Button();
  button.command = command;
  button.y = y;
  mapButtons.put(command, button);
  return y + 45;
}

void mousePressed() {
  
  for (Button button : mapButtons.values()) {
    if (isInsideBox(10,button.y + 20,40)) {
      fill(0,255,0);
      println(button.command);
      RadionicsCommand command = RadionicsCommand.getByName(button.command);
      println(command.getValue());
      arduino.write(command.getValue().toString() + " ARNICA C30" + "\n");
    }
  }
}

boolean isInsideBox(int boxX, int boxY, int boxSize) {

  int size = boxSize / 2;

  return 
    mouseX > boxX - boxSize &&
    mouseX < boxX + boxSize &&
    mouseY > boxY - size &&
    mouseY < boxY + size;
}

class Button {
  String command;
  Integer y;
}

enum RadionicsCommand {
  commands("commands",0),
  analyze("analyze",1),
  broadcast("broadcast",2),
  broadcastpattern("broadcast pattern",3),
  reset("reset",4),
  grounding("grounding",5),
  trng("trng",6),
  clear("clear",7);

  private String name;
  private Integer value;

  RadionicsCommand(String name, Integer value) {
    this.name = name;
    this.value = value;
  }

  public String getName() { return name; }
  public Integer getValue() { return value; }

  public static RadionicsCommand getByName(String name) {
    
    return RadionicsCommand.valueOf(name.toLowerCase().replaceAll(" ","").trim());
  }
}
