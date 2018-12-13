#include <ECVHelpers.h>

void wifiConnect(const char* ssid, const char* password) {
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println(WiFi.localIP());
}
void wifiConnect() {
	wifiConnect("ECV", "esperluette");
}

void post(JsonObject& data, String route, String host) {
	if (WiFi.status() == WL_CONNECTED) {
		//Declare object of class HTTPClient
		HTTPClient http;
		// Prettier data
		char JSONmessageBuffer[300];
		data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
		// connect api
		http.begin(host + route);
		http.addHeader("Content-Type", "application/json");
		// send data
		int httpCode = http.POST(JSONmessageBuffer);
		// <0 if error
		if(httpCode > 0) {
			Serial.printf("[HTTP] POST... code: %d\n", httpCode);
			String payload = http.getString();
			// HTTP return code
			Serial.println(httpCode);
			// request response payload
			Serial.println(payload);
		}
		else {
			Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
		}
		http.end();
		Serial.println("Message envoyé");
	}
	else {
		Serial.printf("[HTTP] Internet not found\n");
	}
}

void post(JsonObject& data, String route) {
	post(data, route, "http://api-naoled.cleverapps.io/");
}
