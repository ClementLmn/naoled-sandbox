
int motionSensorLeft = D7;
int motionSensorRight = D8;

int photoSensorLeft = 0;
int photoSensorRight = 1;

// On défini à partir de quelle valeur la lumière est allumé
int lightIsOn = 20;

void setup() {
    // Photosensor
    pinMode(photoSensorLeft, INPUT_PULLUP);
    pinMode(photoSensorRight, INPUT_PULLUP);
    // Motionsensor
    pinMode(motionSensorLeft, INPUT);
    pinMode(motionSensorRight, INPUT);
}

void loop() {
    long stateMotionSensorLeft = digitalRead(motionSensorLeft);
    long stateMotionSensorRight = digitalRead(motionSensorRight);
    // On vérifie qu'il n'y a aucuns mouvement dans la salle
    if(stateMotionSensorLeft !== HIGH && stateMotionSensorRight !== HIGH){
      checkDataVerification();
    }
    delay(1000);
}

void checkDataVerification(){
  int i = 1;
  while (digitalRead(motionSensorLeft) !== HIGH && digitalRead(motionSensorRight) !== HIGH) {
    if(i > 30){
      // Aucun mouvement n'a été détecté depuis plus de 30 secondes
      // On récupère les données du capteur de lumière
      int statePhotoSensorLeft = analogRead(photoSensorLeft);
      int statePhotoSensorRight = analogRead(photoSensorRight);
      // on formate
      int statePhotoSensorLeftFormate = (statePhotoSensorLeft / 1024.0) * 100;
      int statePhotoSensorRightFormate = (statePhotoSensorRight / 1024.0) * 100;
      // On regarde si la lumière est allumé
      if(statePhotoSensorLeftFormate > lightIsOn && statePhotoSensorRightFormate > lightIsOn){
        // TODO: On envoi la donnée
        pauseWhenNobodyIsHere();
        // TODO: on envoi la donnée comme quoi le problème est résolu
        break;
      }
    }
    i++;
    delay(1000);
  }
}

void pauseWhenNobodyIsHere(){
  // On récupère les données du capteur de lumière
  int statePhotoSensorLeft = analogRead(photoSensorLeft);
  int statePhotoSensorRight = analogRead(photoSensorRight);
  // on formate
  int statePhotoSensorLeftFormate = (statePhotoSensorLeft / 1024.0) * 100;
  int statePhotoSensorRightFormate = (statePhotoSensorRight / 1024.0) * 100;
  // Tant qu'il n'y a pas de mouvement et que la lumière est allumée on me en pause
  while(digitalRead(motionSensorLeft) !== HIGH && digitalRead(motionSensorRight) !== HIGH && statePhotoSensorLeftFormate > lightIsOn && statePhotoSensorRightFormate > lightIsOn){
    delay(1);
  }
}
