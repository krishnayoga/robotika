
#define DRIVER_E 14 // enable
#define DRIVER_D 15 // direction
#define DIRECT_A 16
#define DIRECT_B 17
#define DIRECT_C 18
#define DIRECT_D 19

#define PWMSERVO 9
#define PWMSTEPPER 11

void setup() {
  // put your setup code here, to run once:
  pinMode(DRIVER_D,OUTPUT);
  pinMode(PWMSTEPPER,OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  // HIGH = CW, LOW = CCW
  

  for(int i=0;i<20;i++){

    digitalWrite(DRIVER_D,HIGH);
    
    int startt = millis();
    
    for(int k=0;k<200;k++){
      digitalWrite(PWMSTEPPER,HIGH);
      delayMicroseconds(10000);
      digitalWrite(PWMSTEPPER,LOW);
      delayMicroseconds(10000);

//      Serial.println(k);
    }
    
    int endd = millis() - startt;
    delay(1000);
    
    //min 420
    //total delaymicroseconds * k = time

    Serial.print(endd);
  
    digitalWrite(DRIVER_D,LOW);
  
    for(int k=0;k<200;k++){
      digitalWrite(PWMSTEPPER,HIGH);
      delayMicroseconds(10000);
      digitalWrite(PWMSTEPPER,LOW);
      delayMicroseconds(10000);
    }
  
    delay(1000);

  }

  while(1){};
}
