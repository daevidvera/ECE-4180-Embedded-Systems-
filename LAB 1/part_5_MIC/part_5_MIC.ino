#define MIC 4
#define LED 5

void setup() {
  // put your setup code here, to run once:
  ledcAttach(LED, 5000, 12);
  Serial.begin(11520);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t micLevel = analogRead(MIC);
  Serial.print("The mic level is: "); // Use print() for the first part
  Serial.println(micLevel); 
  int duty = map(micLevel, 1200, 1500, 0, 4095);
  ledcWrite(LED, duty);
  delay(100);

}
