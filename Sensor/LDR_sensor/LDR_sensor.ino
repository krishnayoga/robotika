#define LDR_pin 1

int i;
int t;



void setup() {
  pinMode(LDR_pin,  INPUT);
  Serial.begin(9600);
}

void loop() {

  int LDR = analogRead(LDR_pin);
  
   Serial.print(i);
   Serial.print("/");
   Serial.print(millis());  
   Serial.print("LDR : ");
   Serial.print(LDR);
   Serial.println("");
   delay(50);
   
   i++;
}
