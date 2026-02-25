// Uses library found at: https://github.com/4dsystems/Goldelox-Serial-Arduino-Library/tree/master
// This library can be downloaded directly into the Arduino IDE
// This demo is modified from the GoldeloxBigDemo.ino found in the repo examples

#define DisplaySerial Serial1

#include <HardwareSerial.h>
//#include "Goldelox_Serial_4DLib.h"
//#include "Goldelox_Const4D.h"

// Use this if using HardwareSerial or SoftwareSerial
Goldelox_Serial_4DLib Display(&DisplaySerial);

#include <ezButton.h>
#define ButtUp 6 // Button 1
#define ButtCen 7
#define ButtLeft 0
#define ButtDown 1
#define ButtRight 8
ezButton btnUp(ButtUp);
ezButton btnCen(ButtCen);
ezButton btnLeft(ButtLeft);
ezButton btnDown(ButtDown);
ezButton btnRight(ButtRight);
int x = 64;
int y = 64;
bool atBoundary = false;

int colors[]  = {WHITE, GOLD, BLUE};
const uint8_t numColors = sizeof(colors) / sizeof(colors[0]);
int count = 0;

void setup()
{

    Serial.begin(115200);
    Serial.println("Starting...");

    DisplaySerial.begin(9600, SERIAL_8N1, 4, 5);
    Display.TimeLimit4D = 500;
    Display.gfx_Cls();
    Display.gfx_Circle(64, 64, 20, BLUE);

}

void loop()
{
  Display.gfx_Cls();
  Serial.println("Redrawing Rectangle");
  Display.gfx_Circle(x, y, 10, colors[count]);
  btnUp.loop();
  btnCen.loop();
  btnLeft.loop();
  btnDown.loop();
  btnRight.loop();

  bool Up    = (btnUp.getState() == LOW);
  bool Down  = (btnDown.getState() == LOW);
  bool Cen   = (btnCen.getState() == LOW);
  bool Left  = (btnLeft.getState() == LOW);
  bool Right = (btnRight.getState() == LOW);
    
  if (Up && Left && x > 10 && y > 10) {
    x -= 1;
    y -= 1;
  }

  else if (Up && Right && x < 118 && y > 10) {
    x += 1;
    y -= 1;
  }
  
  else if (Down && Left && x > 10 && y < 118) {
    x -= 1;
    y += 1;
  }

  else if (Down && Right && x < 118 && y < 118) {
    x += 1;
    y += 1;
  }
  else if (Cen) {
      Serial.println("Center Button!");
  }
  else if (Down && y < 118) {
    y += 1;
  }
  else if (Up && y > 10) {
    y -= 1;
  }
  else if (Left && x > 10) {
    x -= 1;
  }
  else if (Right && x < 118) {
    x += 1;
  }

  else if((x == 118 || x == 10 || y == 118 || y == 10) && atBoundary == false)
  {
    count = (count + 1) % numColors;
    Display.gfx_Circle(x, y, 10, colors[count]);
    atBoundary = true;
  }
  else if((x == 118 || x == 10 || y == 118 || y == 10) && atBoundary == true)
  {
    atBoundary = true;
  }
  else { atBoundary = false; }
  delay(50);
}