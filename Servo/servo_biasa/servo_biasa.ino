#include <Servo.h>

Servo myservo;

int derajat;
int incomingByte = 0;

void setup(){
  Serial.begin(9600);
  myservo.attach(9);
}

void loop(){
  if(Serial.available() > 0){
    incomingByte = Serial.read();

    Serial.print("incoming: ");
    Serial.print(incomingByte);

    derajat = incomingByte;

    myservo.write(derajat);
  }
  
}


