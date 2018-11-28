#include <elapsedMillis.h>

elapsedMillis timeElapsed;
unsigned int interval = 30000; // 30s

bool shouldSend = true;
// On défini à partir de quelle valeur la lumière est allumé
int lightIsOn = 20;

int motionSensorLeft = D7;
int motionSensorRight = D8;

int photoSensorLeft = 0;
int photoSensorRight = 1;

void setup() {
  // Photosensor
  pinMode(photoSensorLeft, INPUT_PULLUP);
  pinMode(photoSensorRight, INPUT_PULLUP);
  // Motionsensor
  pinMode(motionSensorLeft, INPUT);
  pinMode(motionSensorRight, INPUT);
}


void loop() {
  if (movement()) {
    timeElapsed = 0;
    shouldSend = true;
  }
  if (timeElapsed > interval && light() && shouldSend) {
      sendData();
      shouldSend = false;
  }
}

bool movement() {
  long stateMotionSensorLeft = digitalRead(motionSensorLeft);
  long stateMotionSensorRight = digitalRead(motionSensorRight);
  if(stateMotionSensorLeft !== HIGH && stateMotionSensorRight !== HIGH){
    return false;
  }else{
    return true;
  }
}

bool light() {
  // On récupère les données du capteur de lumière
  int statePhotoSensorLeft = analogRead(photoSensorLeft);
  int statePhotoSensorRight = analogRead(photoSensorRight);
  // on formate
  int statePhotoSensorLeftFormate = (statePhotoSensorLeft / 1024.0) * 100;
  int statePhotoSensorRightFormate = (statePhotoSensorRight / 1024.0) * 100;
  if(statePhotoSensorLeftFormate > lightIsOn && statePhotoSensorRightFormate > lightIsOn){
    return true;
  }else{
    return false;
  }
}

void sendData() {}
