#include <Button.h>

Button button = Button(13,INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
}

void loop() {
  Serial.println(button.isPressed());
  delay(500);
}
