#include <Button.h>

Button button = Button(0, INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  if(button.uniquePress()){
    // +1 cigarette mise au cendrier
    Serial.println("pressed");
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
  }
}
