#include <Wire.h>
#include <DS1621.h>
#include <ArduinoJson.h>
#include <ECVHelpers.h>

const String id = "MAMAN";

byte addr = (0x90 >> 1) | 0;
DS1621 sensor = DS1621(addr);
int stateDoor = 0;
bool messageStatus = false;

void setup() {
	Serial.begin(115200);
	wifiConnect();
	pinMode(D5, INPUT_PULLUP);
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
	int stateDoor = digitalRead(D5);

	if (stateDoor) {
		while (1) {
			int newStateDoor = digitalRead(D5);
			if (newStateDoor) {
				// La porte est ouverte depuis plus d'une minute
				if (i > 11) {
					int tempAct, tempForm;
					// On récupère la température en centième de degrés
					tempAct = sensor.getHrTemp();
					// On divise par cent pour récupérer la température exacte
					tempForm = tempAct / 100;
					// On vérifie si le radiateur est allumé
					if ((tempForm > tempRadiateur) && !messageStatus) {
						StaticJsonBuffer<300> JSONbuffer;
						JsonObject& data = JSONbuffer.createObject();
						data["name"] = id;
						data["status"] = 1;
						post(data, "doors");
						messageStatus = true;
					}
				}
				delay(delayTime);
			}
			else {
				if (messageStatus) {
					StaticJsonBuffer<300> JSONbuffer;
					JsonObject& data = JSONbuffer.createObject();
					data["name"] = id;
					data["status"] = 0;
					post(data, "doors");
					messageStatus = false;
				}
				Serial.println("La porte est fermée");
				break;
			}
			i++;
		}
	}
	delay(500);
}
