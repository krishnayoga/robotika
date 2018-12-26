#include <Wire.h>
#include <SonarSRF02.h>

#define SRF02_pin (0x70 >> 1)
#define RIGHT_02_ADDRESS (0xE0 >> 1)
SonarSRF02 SRF02(SRF02_pin);
SonarSRF02 RightSonar(RIGHT_02_ADDRESS);

char unit = 'c'; // 'i' for inches, 'c' for centimeters, 'm' for micro-seconds

void setup()
{
  Serial.begin(9600);

  Serial.println("aaa");

  SRF02.begin();
//  isConnected("SRF02 ", SRF02.readVersion());

  RightSonar.begin();
//  isConnected("SRF02-right", RightSonar.readVersion());

  Serial.println("aaaa");
}

void loop()
{
  Serial.println("aa");
  distance("SRF02", SRF02.readRange(unit));
  distance("SRF02-right", RightSonar.readRange(unit));
}

// Print out distance
void distance(String reference, int sensorReading)
{
  Serial.print("Distance from " + reference + ": ");
  Serial.print(sensorReading);
  Serial.print(unit);
  Serial.println();
}

// Print out distance
void isConnected(String reference, int sensorSoft)
{
  if (sensorSoft >= 0)
  {
    Serial.print("Sensor " + reference + " connected (");
    Serial.print(sensorSoft);
    Serial.println(")");
  }
  else
  {
    Serial.println("Sensor " + reference + " not detected...");
  }
}
