#include <Wire.h>
#include <Adafruit_MPR121.h>



Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lastTouched = 0;


void setup()
{ 
   Serial.begin(115200);
  Wire.begin(23, 22);
  // cap.begin(0x5A);
  
  delay(100);
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found");
    while (1);
  }
  cap.setAutoconfig(true);

  Serial.println("Keypad ready");
}


void loop()
{
  uint16_t touched = cap.touched();


  for (uint8_t i = 0; i < 12; i++) {
    if ((touched & (1 << i)) && !(lastTouched & (1 << i))) {
      Serial.print("Pressed electrode: ");
      Serial.println(i);
    }
  }

  lastTouched = touched;
  delay(50);
}




