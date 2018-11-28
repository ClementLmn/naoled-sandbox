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
	// On vérifie si la porte reste ouverte toutes les 5 secondes
  int delayTime = 5000;
	// On défini à partir de quelle température on considère le radiateur comme "allumé"
	int tempRadiateur = 20;

	int i = 1;
	int stateDoor = digitalRead(0);
  if(stateDoor){
    while (1) {
      int newStateDoor = digitalRead(0);
      if(newStateDoor){
				// La porte est ouverte depuis plus d'une minute
        if(i > 11){
					int tempAct, tempForm;
					// On récupère la température en centième de degrés
					tempAct = sensor.getHrTemp();
					// On divise par cent pour récupérer la température exacte
					tempForm = tempAct / 100;
					// On vérifie si le radiateur est allumé
					if(tempForm > tempRadiateur){
						Serial.print("La porte est ouverte depuis plus d'une minute et le radiateur est allumé !");
					}
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
