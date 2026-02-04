
#include <ESP32Servo.h>

#define BUTTON1 4
#define BUTTON2 5

Servo myservo;



void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  myservo.attach(6);
  Serial.begin(11520);


}

void loop() {
  // put your main code here, to run repeatedly:

  if (!digitalRead(BUTTON1)) {
    if (!digitalRead(BUTTON2)) {
       Serial.println("Input Position = 45"); 
       myservo.write(45);
    } else {
       Serial.println("Input Position = 90");
       myservo.write(90);

    }
  } else if (digitalRead(BUTTON1)) {

     if (!digitalRead(BUTTON2)) {
       Serial.println("Input Position = 135"); 
       myservo.write(135);
    } else {
       Serial.println("Input Position = 180"); 
       myservo.write(180);
    }


  }

}
