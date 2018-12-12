#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

void wifiConnect(const char* ssid, const char* password);
void wifiConnect();
void post(JsonObject& data, String route, String host);
void post(JsonObject& data, String route);
