#include <dht.h>

dht DHT;

#define DHT_pin 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int chk = DHT.read22(DHT_pin);

  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.print(DHT.temperature,1);
  Serial.println();
}
