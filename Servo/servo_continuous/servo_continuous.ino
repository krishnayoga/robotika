#include <Servo.h>

Servo myservo; 
 
int incomingByte = 0;  

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    
    Serial.print("incoming: ");
    Serial.print (incomingByte);
    if(incomingByte == 100){
      Serial.println(" CW 0 derajat "); 
      myservo.write(0); 
    }
    else if(incomingByte == 200){
      Serial.println(" CCW 180 derajat "); 
      myservo.write(180); 
    }
    else if(incomingByte == 300){
      Serial.println(" Stop "); 
      myservo.write(60); 
    }
    else{
      Serial.println(" Jalan ngaco "); 
      myservo.write(incomingByte); 
    }
  }
} 
