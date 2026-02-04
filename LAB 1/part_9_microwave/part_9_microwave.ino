#define POT 4 // potentiometer
#define PWM 5
#define AI1 6
#define AI2 7



enum state_nodes = {START, COOKING, PAUSED, DONE};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Enter a number: ")
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(PWM, OUTPUT);

  enum state_codes current_state = START;
}

void loop() {

  // put your main code here, to run repeatedly:
  uint16_t potValue = analogRead(POT);
  int newPot = map(potValue, 12 ,3300, 0, 255);
  int newPotValue = abs(newPot - 122);
  newPotValue = map(newPotValue,0, 122, 0, 255);
  newPotValue = constrain(newPotValue, 0, 255);
  analogWrite(PWM, newPotValue); 
  Serial.println(newPotValue);

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
     int time = Serial.parseInt();
     current_state = COOKING;
     break;

    case COOKING:
    while(time >= 0){

      if(POT <= 0) {
        current_state = PAUSED;
        break;
      }
      
      Serial.println(time);
      time = time -1;
      delay(100);

    }
    current_state = DONE;
      

    break;

    case PAUSED:
    break;

    case DONE:
    break;
    
    
  }


  
  delay(50);
}
