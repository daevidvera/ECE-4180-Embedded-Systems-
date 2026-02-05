#include <Adafruit_NeoPixel.h>
#define NUM_LEDS    1     // Only one LED on board
#define POT 4 // potentiometer
#define PWM 5
#define AI1 6
#define AI2 7
#define LED_PIN     8     // Data pin connected to onboard RGB LED
//define speaker pin?

enum state_nodes {START, COOKING, PAUSED, DONE, STANDBY};
enum state_nodes current_state = START;
int cookTime = 0;  // Global variable
Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pixel.begin(); 
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(PWM, OUTPUT);
  //speaker pin??

}

void loop() {

  uint16_t potValue = analogRead(POT); //POT INPUT

  switch (current_state) {

    case START:
     pixel.setPixelColor(0, pixel.Color(0, 0, 255)); // Display Red// LED turns blue
     pixel.show();
     Serial.println("Enter a number: ");
     while (Serial.available() == 0){
      delay(10);
     }
     cookTime = Serial.parseInt();
     while(Serial.available() > 0) {
        Serial.read();
      }
      if(analogRead(POT) < 12)
      {
        current_state = STANDBY;
      }
     if (cookTime > 0){
      current_state = COOKING;
     }
     break;

    case COOKING:
      pixel.setPixelColor(0, pixel.Color(255, 255, 0)); // Display Red//yellow LED
      pixel.show();
      analogWrite(PWM, 100);
      digitalWrite(AI1, LOW);
      digitalWrite(AI2, HIGH);
      //add conditional of pot turning or door opening if so then current state becomes PAUSED
      while(cookTime > 0){
        if(analogRead(POT) < 20) {
          current_state = PAUSED;
          break;
        }
        Serial.println(cookTime);
        cookTime = cookTime -1;
        delay(1000);

      }
      if (cookTime == 0){
        current_state = DONE;
      }
      
      break;

    case PAUSED:
    pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // Display Red
    pixel.show();
    analogWrite(PWM, 0);
    while(analogRead(POT) < 3000)
    {
      delay(10);
    }
    current_state = COOKING;
    break;

    case DONE:
      Serial.println("DONE!");
      pixel.setPixelColor(0, pixel.Color(0, 255, 0)); // Display  turns green
      pixel.show();
      analogWrite(PWM, 0);
      // speaker beeps
      //make condtional on waiting for door to open or pot
      if(analogRead(POT) < 12)
      {
        current_state = STANDBY;
      }
    break;

    case STANDBY:
      pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // Display  turns green
      pixel.show();
      if(analogRead(POT) > 3000)
      {
        current_state = START;
      }
  }

  
  delay(50);
}