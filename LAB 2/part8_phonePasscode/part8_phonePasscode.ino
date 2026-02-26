#define DisplaySerial Serial1
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"
#include <Wire.h>
#include <Adafruit_MPR121.h>
#include "ICM_20948.h"

Goldelox_Serial_4DLib Display(&DisplaySerial);
Adafruit_MPR121 cap = Adafruit_MPR121();
ICM_20948_I2C myICM;

// ---- CHANGE THIS TO YOUR BOARD NUMBER ----
const String DIGIT_CODE = "0032";
// Tilt sequence: 0=Forward(accY>500), 1=Backward(accY<-500), 2=Left(accX<-500)
const int TILT_CODE[]   = {0, 1, 2};
const int TILT_SEQ_LEN  = 3;

// State
String digitInput   = "";
int tiltInput[3]    = {-1, -1, -1};
int tiltCount       = 0;
bool digitsDone     = false;
bool unlocked       = false;
bool tiltBoundary   = false;
uint16_t lastTouched = 0;

// ---- Tilt direction helper ----
int getTiltDirection() {
  if (!myICM.dataReady()) return -1;
  myICM.getAGMT();
  if (myICM.accY() > 500)  return 0; // forward
  if (myICM.accY() < -500) return 1; // backward
  if (myICM.accX() < -500) return 2; // left
  return -1;
}

// ---- Display helpers ----
void drawScreen() {
  Display.gfx_Cls();
  delay(50);

  if (unlocked) {
    // Green UNLOCKED
    Display.txt_FGcolour(GREEN);
    Display.txt_MoveCursor(2, 1);
    Display.print("  UNLOCKED!");
    Display.txt_MoveCursor(4, 1);
    Display.txt_FGcolour(WHITE);
    Display.print("Welcome :)");
  } else {
    // Red LOCKED
    Display.txt_FGcolour(RED);
    Display.txt_MoveCursor(1, 2);
    Display.print("  LOCKED");

    // Show digit input progress
    Display.txt_FGcolour(WHITE);
    Display.txt_MoveCursor(3, 0);
    Display.print("PIN: ");
    for (int i = 0; i < 4; i++) {
      if (i < (int)digitInput.length()) {
        Display.print(String(digitInput[i]));
      } else {
        Display.print("-");
      }
    }

    // Show tilt input progress (only after digits done)
    if (digitsDone) {
      Display.txt_MoveCursor(5, 0);
      Display.print("TILT: ");
      for (int i = 0; i < TILT_SEQ_LEN; i++) {
        if (i < tiltCount) {
          if (tiltInput[i] == 0) Display.print("^");  // forward
          else if (tiltInput[i] == 1) Display.print("v"); // backward
          else if (tiltInput[i] == 2) Display.print("<"); // left
        } else {
          Display.print("-");
        }
      }
    } else {
      Display.txt_MoveCursor(5, 0);
      Display.txt_FGcolour(YELLOW);
      Display.print("Enter PIN first");
    }
  }
  delay(100);
}

void checkUnlock() {
  // Check digits
  if (digitInput != DIGIT_CODE) {
    // Wrong pin — reset everything
    digitInput  = "";
    tiltCount   = 0;
    digitsDone  = false;
    Display.gfx_Cls();
    Display.txt_FGcolour(RED);
    Display.txt_MoveCursor(3, 1);
    Display.print("Wrong PIN!");
    delay(1500);
    drawScreen();
    return;
  }

  // Check tilts
  for (int i = 0; i < TILT_SEQ_LEN; i++) {
    if (tiltInput[i] != TILT_CODE[i]) {
      // Wrong tilt — reset everything
      digitInput = "";
      tiltCount  = 0;
      digitsDone = false;
      Display.gfx_Cls();
      Display.txt_FGcolour(RED);
      Display.txt_MoveCursor(3, 1);
      Display.print("Wrong Tilt!");
      delay(1500);
      drawScreen();
      return;
    }
  }

  // All correct!
  unlocked = true;
  drawScreen();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // I2C for keypad + IMU on same bus
  Wire.begin(23, 22); // SDA=GPIO23, SCL=GPIO22

  // Init MPR121 keypad
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found");
    while (1);
  }
  cap.setAutoconfig(true);
  Serial.println("Keypad ready");

  // Init IMU
  bool initialized = false;
  while (!initialized) {
    myICM.begin(Wire, 1);
    if (myICM.status == ICM_20948_Stat_Ok) {
      initialized = true;
    } else {
      Serial.println("IMU not found, retrying...");
      delay(500);
    }
  }
  Serial.println("IMU ready");

  // Init uLCD
  DisplaySerial.begin(9600, SERIAL_8N1, 4, 5);
  Display.TimeLimit4D = 500;
  delay(5000); // wait for display boot
  drawScreen();
}

void loop() {
  if (unlocked) return; // do nothing once unlocked

  // ---- Handle Keypad ----
  uint16_t touched = cap.touched();
  for (uint8_t i = 0; i < 12; i++) {
    if ((touched & (1 << i)) && !(lastTouched & (1 << i))) {
      Serial.print("Key: "); Serial.println(i);

      if (!digitsDone) {
        if (i <= 9) {
          // digit input
          if (digitInput.length() < 4) {
            digitInput += String(i);
            drawScreen();
          }
        } else if (i == 10) {
          // enter — move to tilt phase if 4 digits entered
          if (digitInput.length() == 4) {
            digitsDone = true;
            drawScreen();
          }
        } else if (i == 11) {
          // delete
          if (digitInput.length() > 0) {
            digitInput.remove(digitInput.length() - 1);
            drawScreen();
          }
        }
      }
    }
  }
  lastTouched = touched;

  // ---- Handle Tilt (only after digits entered) ----
  if (digitsDone && tiltCount < TILT_SEQ_LEN) {
    int dir = getTiltDirection();
    if (dir != -1 && !tiltBoundary) {
      tiltInput[tiltCount] = dir;
      tiltCount++;
      tiltBoundary = true;
      drawScreen();

      // All tilts entered — check unlock
      if (tiltCount == TILT_SEQ_LEN) {
        delay(500);
        checkUnlock();
      }
    } else if (dir == -1) {
      tiltBoundary = false; // reset when back to neutral
    }
  }

  delay(50);
}