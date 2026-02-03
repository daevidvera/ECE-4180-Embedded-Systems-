#include <Adafruit_NeoPixel.h>

#define LED_PIN     8     // Data pin connected to onboard RGB LED
#define NUM_LEDS    1     // Only one LED on board

#define BUTTON 4 // Button 1
#define BUTTON2 5
Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int count = 0;

bool lastbtn1 = HIGH;
bool lastbtn2 = HIGH;

int colors[]  = {pixel.Color(255, 0, 0), pixel.Color(0, 255, 0), pixel.Color(0,0, 255), pixel.Color(246, 190, 0)};
const uint8_t numColors = sizeof(colors) / sizeof(colors[0]);
void setup() {
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON, INPUT_PULLUP);
    pixel.begin();          
    Serial.begin(115200);
}

void loop() {

    bool b1 = digitalRead(BUTTON);
    bool b2 = digitalRead(BUTTON2);
    
    if (lastbtn1 == HIGH && b1 == LOW) {
        count = (count + 1) % numColors;
        pixel.setPixelColor(0, colors[count]);
        pixel.show();
    }

      if (lastbtn2 == HIGH && b2 == LOW) {
        count = (count + numColors - 1) % numColors;
        pixel.setPixelColor(0, colors[count]);
        pixel.show();
    }

    lastbtn1 = b1;
    lastbtn2 = b2;





    }


