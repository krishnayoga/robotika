#define PIR_pin 4

void setup()
{
  Serial.begin(9600);
  pinMode(PIR_pin, INPUT);
}

void loop() 
{
  if(detectOrang())
    Serial.println("Ada Orang");
  else
    Serial.println("Tidak Ada Orang");
}

boolean detectOrang()
{
  int sensorValue = digitalRead(PIR_pin);
  if(sensorValue == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}
