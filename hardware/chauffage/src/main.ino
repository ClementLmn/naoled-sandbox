#include <Wire.h>
#include <DS1621.h>

byte addr = (0x90 >> 1) | 0;
DS1621 sensor=DS1621(addr);

void setup() {
	Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);

  sensor.startConversion(false);
  sensor.setConfig(DS1621::POL | DS1621::ONE_SHOT);

  Wire.begin(D1, D2);
}

void loop() {
  int stateDoor = digitalRead(0);

  int delayTime = 5000;
  int i = 1;
  if(stateDoor){
    while (1) {
      int newStateDoor = digitalRead(0);

      if(newStateDoor){
        int tempAct, tempForm;
        // On récupère la température en centième de degrés
        tempAct = sensor.getHrTemp();
        // On divise par cent pour récupérer la température exacte
        tempForm = tempAct / 100;

        if(i > 10){
          Serial.print("La porte est ouverte depuis trop longtemps ! Il fait actuellement ");
          Serial.print(tempForm);
          Serial.println("° C.");
        }else if(i == 1){
          Serial.print("La porte viens de s'ouvrir et il fait actuellement ");
          Serial.print(tempForm);
          Serial.println("° C.");
        }else{
          int timeForm = (delayTime / 1000) * i;
          Serial.print("La porte est ouverte depuis ");
          Serial.print(timeForm);
          Serial.print(" secondes et il fait actuellement ");
          Serial.print(tempForm);
          Serial.println("° C.");
        }
        delay(delayTime);
      }else{
        Serial.println("La porte est fermé");
        break;
      }
      i++;
    }
  }
  delay(500);
}
