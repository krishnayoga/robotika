#include <Wire.h>

// Address of SRF02 range finder on I2C
#define ADDRESS 0x70
 
void setup()
{
  // Connection to the connected Laptop
  Serial.begin(9600);
  
  Wire.begin();
}
 
void loop()
{
  //Allocate Memory
  byte byteHigh, byteLow;
  int distance;

  Wire.beginTransmission(ADDRESS); // begin communication with SFR02
  Wire.write(byte(0x00));          // sets register pointer to the command register (0x00)
  Wire.write(byte(0x51));          // command sensor to measure in centimeters
  Wire.endTransmission();          // end transmission

  // Wait for 70 milliseconds
  delay(70); 
  
  Wire.beginTransmission(ADDRESS); // begin communication with SFR02
  Wire.write(byte(0x02));          // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();          // end transmission
   
  Wire.requestFrom(ADDRESS, 2);    // Request 2 bytes from SFR02
  
  while(Wire.available() < 2);     // Wait for the bytes to arrive
  
  // Read the values
  byteHigh = Wire.read(); 
  byteLow = Wire.read();
 
  // Calculate full bearing
  distance = ((byteHigh<<8) + byteLow);

  // Print data to Serial Monitor window
  Serial.print("$Distance,");
  Serial.println(distance, DEC);
  
  // In the demo wait half a second
  delay(500);
}
