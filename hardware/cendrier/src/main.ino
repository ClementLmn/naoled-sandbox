#include <Button.h>

// NAME : EYJAFJALLAJOKULL

Button button = Button(D4, INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
  pinMode(D5, OUTPUT);
}

void loop() {
  if(button.uniquePress()){
    // +1 cigarette mise au cendrier
    Serial.println("pressed");
    // On envoi
    goLight();
  }
}

void goLight() {
  digitalWrite(D5, HIGH);
  delay(1000);
  digitalWrite(D5, LOW);
}
