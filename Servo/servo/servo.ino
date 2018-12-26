#define servoPin 9

void setup() {
  // put your setup code here, to run once:
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(servoPin,HIGH);
  delayMicroseconds(2100);
  digitalWrite(servoPin,LOW);
  delayMicroseconds(16000);

  delay(1000);
}
