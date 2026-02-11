#include <Adafruit_MCP23X17.h>

#define BUTTON_PIN 2 
#define LED_PIN 1


// esp32 boards
#define CS_PIN 4  
#define MOSI_PIN 5
#define MISO_PIN 6
#define SCK_PIN 7 



Adafruit_MCP23X17 mcp; 




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  

  if (!mcp.begin_SPI(CS_PIN, SCK_PIN, MISO_PIN, MOSI_PIN, 0)) {
    Serial.println("Error. ");
    while(1);
  }
  mcp.pinMode(BUTTON_PIN, INPUT_PULLUP);
  mcp.pinMode(LED_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  if (!mcp.digitalRead(BUTTON_PIN)) {
    mcp.digitalWrite(LED_PIN, HIGH);
  } else {
    mcp.digitalWrite(LED_PIN, LOW);
  }

}
