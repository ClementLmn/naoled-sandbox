#include <elapsedMillis.h>

// NAME : TESLA

elapsedMillis timeElapsed;
unsigned int interval = 30000; // 30s

bool shouldSend = true;
// On défini à partir de quelle valeur la lumière est allumé
int lightIsOn = 80;

int motionSensor = D7;

int photoSensor = 0;
// int photoSensorRight = 1;

void setup() {
  // Photosensor
  Serial.begin(115200);
  pinMode(photoSensor, INPUT_PULLUP);
  // pinMode(photoSensorRight, INPUT_PULLUP);
  // Motionsensor
  pinMode(motionSensor, INPUT);
}


void loop() {
  movement();
  light();
  if (movement()) {
    timeElapsed = 0;
    shouldSend = true;
  }
  if (timeElapsed > interval && light() && shouldSend) {
      sendData();
      shouldSend = false;
  }
  Serial.println("Loop Fini");
  delay(1000);
}

bool movement() {
  long stateMotionSensor = digitalRead(motionSensor);
  Serial.print("Mouvement : ");
  Serial.println(stateMotionSensor);
  if(stateMotionSensor != HIGH){
    return false;
  }else{
    return true;
  }
}

bool light() {
  // On récupère les données du capteur de lumière
  int statePhotoSensor = analogRead(photoSensor);
  // on formate
  int statePhotoSensorFormate = (statePhotoSensor / 1024.0) * 100;
  Serial.print("Light : ");
  Serial.println(statePhotoSensorFormate);
  if(statePhotoSensorFormate > lightIsOn){
    return true;
  }else{
    return false;
  }
}

void sendData() {
  Serial.print("Message envoyé");
}
