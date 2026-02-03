#include <ezButton.h>
#define ButtUp 4 // Button 1
#define ButtCen 5
#define ButtLeft 6
#define ButtDown 7
#define ButtRight 8
ezButton btnUp(ButtUp);
ezButton btnCen(ButtCen);
ezButton btnLeft(ButtLeft);
ezButton btnDown(ButtDown);
ezButton btnRight(ButtRight);


void setup() {  

    btnUp.setDebounceTime(100);
    btnCen.setDebounceTime(100);
    btnLeft.setDebounceTime(100);
    btnDown.setDebounceTime(100);
    btnRight.setDebounceTime(100);

    Serial.begin(115200);
}

void loop() {
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
    
    if (Up && Left) {
        Serial.println("top-left");
    }

    else if (Up && Right) {
        Serial.println("top-right");
    }
    
    else if (Down && Left) {
        Serial.println("bottom-left");
    }

    else if (Down && Right) {
        Serial.println("bottom-right");
    }
    else if (Cen) {
        Serial.println("Center Button!");
    }
    else if (Down) {
        Serial.println("Down Button!");
    }
    else if (Up) {
        Serial.println("Up Button!");
    }
    else if (Left) {
        Serial.println("Left Button!");
    }
    else if (Right) {
        Serial.println("Right Button!");
    }
    delay(20);
}
