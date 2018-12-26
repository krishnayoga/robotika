#define GP_sensor 2

void setup() {
  // put your setup code here, to run once:
  pinMode(GP_sensor, INPUT);
  Serial.begin(9600);
}
int i;
void loop() {
  // put your main code here, to run repeatedly:
  int GP = analogRead(GP_sensor);
  Serial.print(i);
  Serial.print("/");
  Serial.print(millis());  
  Serial.print("GP : ");
  Serial.println(GP);
  delay(50);
   
  i++;
}
