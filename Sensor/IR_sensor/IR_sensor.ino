
int photoDiode = 2;                                       
int sensorRead = 3;                  
int sensor_thr = 905;   
                
void setup()    
{  
  pinMode(photoDiode,INPUT);  
  digitalWrite(photoDiode,HIGH);          
  Serial.begin(9600);           
}  
void loop()  
{  
  int val = analogRead(sensorRead);    
  Serial.println(val);            
}  
