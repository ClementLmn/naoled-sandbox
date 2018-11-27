#include <Wire.h>
#include <DS1621.h>

byte addr = (0x90 >> 1) | 0;
DS1621 sensor=DS1621(addr);

void setup()
{
  sensor.startConversion(false);
  sensor.setConfig(DS1621::POL | DS1621::ONE_SHOT);

  Serial.begin(115200);
  // On indique les port du nodemcu
  Wire.begin(D1, D2);
}


void loop()
{
  int tC, tFrac;

  // On récupère la température en centième de degrés
  tC = sensor.getHrTemp();
  // On divise par cent pour récupérer la température exacte
  tC = tC / 100;
  Serial.print(tC);

  delay(500);
}
