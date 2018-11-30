#include <Wire.h>
#include <DS1621.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : MAMAN

byte addr = (0x90 >> 1) | 0;
DS1621 sensor=DS1621(addr);

void setup() {
	Serial.begin(115200);
	WiFi.begin("Honor 5C", "azertyuiop");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

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
						StaticJsonBuffer<300> JSONbuffer;
						JsonObject& listData = JSONbuffer.createObject();
						listData["name"] = "MAMAN";
						sendData(listData);
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

void sendData(listData) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listData.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    // connect api
    http.begin("http://api-naoled.cleverapps.io/ADDWARM");
    http.addHeader("Content-Type", "application/json");
    // send data
    int httpCode = http.POST(JSONmessageBuffer);
    // <0 if error
    if(httpCode > 0){
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      String payload = http.getString();
      // HTTP return code
      Serial.println(httpCode);
      // request response payload
      Serial.println(payload);
    }else{
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    Serial.print("Message envoyé");
  }else{
    Serial.printf("[HTTP} Internet not found\n");
  }
}
