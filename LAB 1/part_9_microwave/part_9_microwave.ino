#define POT 4 // potentiometer
#define PWM 5
#define AI1 6
#define AI2 7
//define speaker pin?

enum state_nodes {START, COOKING, PAUSED, DONE};
enum state_nodes current_state = START;
int cookTime = 0;  // Global variable


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(PWM, OUTPUT);
  //speaker pin??

}

void loop() {

  // put your main code here, to run repeatedly:
  // uint16_t potValue = analogRead(POT); //POT INPUT
  // analogWrite(PWM, 50);  // PWM OUTPUT
  // Serial.println(newPotValue);

  /// POT AND MOTOR DRIVER LOGIC
  // if(newPot > 122)
  // {
  //   digitalWrite(AI1, HIGH);
  //   digitalWrite(AI2, LOW);
  // }
  // else if(newPot < 122)
  // {
  //   digitalWrite(AI1, LOW);
  //   digitalWrite(AI2, HIGH);
  // }
  // else
  // {
  //   digitalWrite(AI1, LOW);
  //   digitalWrite(AI2, LOW);
  // }

  switch (current_state) {

    case START:
     // LED turns blue
     Serial.println("Enter a number: ");
     while (Serial.available() == 0){
      delay(10);
     }
     cookTime = Serial.parseInt();
     while(Serial.available() > 0) {
        Serial.read();
      }
     if (cookTime > 0){
      current_state = COOKING;
     }
     break;

    case COOKING:
      //yellow LED
      //add conditional of pot turning or door opening if so then current state becomes PAUSED
      while(cookTime > 0){

        if(POT <= 0) {
          current_state = PAUSED;
          break;
        }
        
        Serial.println(cookTime);
        cookTime = cookTime -1;
        delay(1000);

      }
      current_state = DONE;
      break;

    case PAUSED:
    // led turns red
    // waits until pot to close again and then goes back to COOKING state
    break;

    case DONE:
      Serial.println("DONE!");
      //LED turns green
      // speaker beeps
      //make condtional on waiting for door to open or pot
      // then LED turns red
      // then waits for door to close or pot turn again to go to start state
      current_state = START;
    break;
  }

  
  delay(50);
}