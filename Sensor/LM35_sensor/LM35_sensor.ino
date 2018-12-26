

float temp_lm35;
int tempPin = 0;
int i;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  i++;
  temp_lm35 = analogRead(tempPin);
  temp_lm35 = temp_lm35 * 0.48828125;

  Serial.print(i);
  Serial.print(" ");
  Serial.print("TEMPRATURE = ");
  Serial.print(temp_lm35);
  Serial.println();

  delay(50);
}
