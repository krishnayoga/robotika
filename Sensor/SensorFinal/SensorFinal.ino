#include "RTClib.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <SD.h>
#include <MPU9255.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_Millis rtc;
MPU9255 mpu;

#define LM35pin 0
#define LDRpin 1
#define GPpin 2

#define IRextpin 7
#define Inductivepin 6
#define IRLpin 5
#define PIRpin 4
#define IRpairpin 3
#define DHTpin 2
#define DHTTYPE DHT22
DHT dht(DHTpin, DHTTYPE);

#define I2C_LCD 0x27
#define SRFAddress 0x70
#define RTCAddress 0x68

#define byte uint8_t

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const int chipSelect = 10;
String dataString = "";
//String dataString2 = "";
int sensor='1';

void waktu()
{
  DateTime now = rtc.now();
    lcd.setCursor(0,0); 
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    lcd.print(now.day(), DEC);
    lcd.print(now.month(), DEC);
    lcd.print(now.year(), DEC);   
}
void setup() 
{
 //SDCard
 Serial.begin(9600);
 while (!Serial) 
 {
    ;
 }
 Serial.print("Initializing SD card...");
 if (!SD.begin(chipSelect)) 
 {
  Serial.println("Card failed, or not present");
  return;
 }
 Serial.println("card initialized.");
 Serial.begin(9600);
 lcd.begin();
 dht.begin();
 Wire.begin();
 mpu.init();
// Serial.begin(115200);
 rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() 
{
  //LM35 Temperature
  float readLM35 = analogRead(LM35pin);
  float mv = (readLM35/1024.0)*5000;
  float cel = mv/10;
 
//  Serial.print("LM35 : ");
//  Serial.print(cel);
//  Serial.print(" C");

  //LDR
  float readLDR = analogRead(LDRpin);
//  Serial.print("  LDR : ");
//  Serial.print(readLDR);
                         
  //GP                         
  float readGP = analogRead(GPpin);
//  Serial.print("  GP : ");
//  Serial.print(readGP);
  
  //IRpair
  int readIRpair = digitalRead(IRpairpin);
//  Serial.print("  IRpair : ");
//  if(readIRpair == LOW)
//    {
//     Serial.print("ON");
//    }
//  else
//    {
//     Serial.print("-");
//    }
  
  //PIR 
  int readPIR;
  readPIR = digitalRead(PIRpin);
//  Serial.print("  PIR : ");
//  if(readPIR == HIGH)
//    {
//      Serial.print("ON");
//    }
//  else
//    {
//      Serial.print("-");
//    }
  
  //IRL
  int readIRL = digitalRead(IRLpin);
//  Serial.print("  IRL : ");
//  if(readIRL == HIGH)
//    {
//      Serial.print("ON");
//    }
//  else
//    {
//      Serial.print("-");
//    }
  
  //Inductive
  int readInd = digitalRead(Inductivepin);
//  Serial.print("  Ind : ");
//  if(readInd == HIGH)
//    {
//      Serial.print("ON");  
//    }
//  else
//    {
//      Serial.print("-");
//    }
      
  //IRext 
  int readIRext = digitalRead(IRextpin);
//  Serial.print("  IRext : ");
//  if(readIRext == LOW)
//    {
//      Serial.print("ON");  
//    }
//  else
//    {
//      Serial.print("-");
//    }
  
  //SRF
  byte byteHigh, byteLow;
  int distance;
  Wire.beginTransmission(SRFAddress); 
  Wire.write(byte(0x00));             
  Wire.write(byte(0x51));             
  Wire.endTransmission();             
   
  Wire.beginTransmission(SRFAddress); 
  Wire.write(byte(0x02));             
  Wire.endTransmission();             
   
  Wire.requestFrom(SRFAddress, 2);    
  while(Wire.available() < 2);        

  byteHigh = Wire.read(); 
  byteLow = Wire.read();

  distance = ((byteHigh<<8) + byteLow);

//  Serial.print(" $Distance, ");
//  Serial.print(distance, DEC);
  
  //DHT() 
  int readDHT = digitalRead(DHTpin);
  int chk;
  float hum; // humidity
  float temp; // temperature

  hum = dht.readHumidity();
  temp = dht.readTemperature();

//  Serial.print(" Humidity:  ");
//  Serial.print(hum);
//  Serial.print(" %, Tempt:  ");
//  Serial.print(temp);
//  Serial.println(" Celcius ");

  //MPU-Gyro
  mpu.read_acc();
  mpu.read_gyro();
  mpu.read_mag();

  float ax=mpu.ax;
  float ay=mpu.ay;
  float az=mpu.az;

  float gx=mpu.gx;
  float gy=mpu.gy;
  float gz=mpu.gz;

  float mx=mpu.mx;
  float my=mpu.my;
  float mz=mpu.mz;

    //Acceleration
    ax=ax/16384;
    ay=ay/16384;
    az=(az/16384);

    //Gyroscope
    gx=gx/131;
    gy=gy/131;
    gz=gz/131;

    //Magnetic Flux
    const float cal=0.06;
    mx=mx*cal;
    my=my*cal;
    mz=mz*cal;
    mx=mx/0.6;
    my=my/0.6;
    mz=mz/0.6;
    
  String dataString="";
//  dataString+=String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + "--" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  dataString+="LM35: " + String(cel) + " C" + " | ";
  dataString+="LDR: " + String(readLDR) + " | ";
  dataString+="GP: " + String(readGP) + " | ";
  dataString+="IRpair: ";
     if(readIRpair == LOW)
     {
      dataString+="ON";
     }
     else
     {
      dataString+=("-");
     }
  dataString+=" | ";
  dataString+="PIR: ";
     if(readPIR == HIGH)
     {
      dataString+="ON";
     }
     else
     {
      dataString+=("-");
     }
  dataString+=" | ";
  dataString+="IRL: ";
     if(readIRL == HIGH)
     {
      dataString+="ON";
     }
     else
     {
      dataString+=("-");
     }
  dataString+=" | ";
  dataString+="Inductive= ";
     if(readInd == LOW)
     {
      dataString+="ON";
     }
     else
     {
      dataString+=("-");
     }
  dataString+=" | ";
  dataString+="IRext: ";
     if(readIRext == LOW)
     {
      dataString+="ON";
     }
     else
     {
      dataString+=("-");
     }
  dataString+=" | ";
  dataString+="SRF: " + String(distance) + " | ";
  //DHT
  dataString+="Humidity: " + String(hum) + " % - ";
  dataString+="Temp: " + String(temp) + " C";
  dataString+=" | ";

//  String dataString2="";
//  //Acceleration
//  dataString2+="Acceleration: ";
//  dataString2+=String(ax) + " / ";
//  dataString2+=String(ay) + " / ";
//  dataString2+=String(az);
//  //Gyroscope
//  dataString2+="Gyroscope: ";
//  dataString2+=String(gx) + " / ";
//  dataString2+=String(gy) + " / ";
//  dataString2+=String(gz);
//  //Magnetic Flux
//  dataString2+="Magnetic Flux: ";
//  dataString2+=String(mx) + " / ";
//  dataString2+=String(my) + " / ";
//  dataString2+=String(mz);

  //CASE SENSOR
  //LM35
  switch(sensor)
  {
  case '1':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print("LM35 : ");
    lcd.print(cel);
    lcd.print(" C");
    break;
  //LDR
  case '2':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print("  LDR : ");
    lcd.print(readLDR);
    break;
  //GP
  case '3':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print("  GP : ");
    lcd.print(readGP);
    break;
  //IRpair
  case '4':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
        lcd.print("  IRpair : ");
        if(readIRpair == LOW)
          {
           lcd.print("ON");
          }
          else
          {
          lcd.print("-");
          }
     break;
  //PIR
  case '5':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
        lcd.print("  PIR : ");
        if(readPIR == HIGH)
          {
           lcd.print("ON");
          }
          else
          {
           lcd.print("-");
          }  
     break;
  //IRL
  case '6':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
        lcd.print("  IRL : ");  
        if(readIRL == HIGH)
          {
           lcd.print("ON");
          }
          else
          {
           lcd.print("-");
          }
    break;
  //Inductive
  case '7':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
        lcd.print("  Ind : ");
        if(readInd == HIGH)
          {
           lcd.print("ON");  
          }
          else
          {
           lcd.print("-");
          }
    break;
  //IRext
  case '8':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
        lcd.print("  IRext : ");
        if(readIRext == LOW)
          {
           lcd.print("ON");  
          }
          else
          {
           lcd.print("-");
          }
    break;
  //SRF
  case '9':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print("Distance : ");
    lcd.print(distance, DEC);
    break;
  //DHT
  case 'q':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print(hum);
    lcd.print("% ");
    lcd.print(temp);
    lcd.println("C ");
//    lcd.scrollDisplayLeft();
    break;

  //MPU - Gyro (Accel)
  case 'w':
    lcd.clear();
//    waktu();
    lcd.setCursor(0,1);
    lcd.print(ax);
    lcd.print(" ");
    lcd.print(ay);
    lcd.print(" ");
    lcd.print(az);
    break;

  //MPU - Gyro (Gyroscope)
  case 'e':
    lcd.clear();
    waktu();
    lcd.setCursor(0,1);
    lcd.print(gx);
    lcd.print(" ");
    lcd.print(gy);
    lcd.print(" ");
    lcd.print(gz);
    break;

  //MPU - Gyro (Magnetic Flux)
  case 'r':
    lcd.clear();
    waktu();
    lcd.print(mx);
    lcd.print(" ");
    lcd.print(my);
    lcd.print(" ");
    lcd.print(mz);
    break;
    
  case '0':
    lcd.clear();
//    lcd.setCursor(0,0);
//     lcd.print("Distance : ");
//    lcd.print(distance,DEC);
//    lcd.clear();
    lcd.setCursor(0,1);
     lcd.print("   LDR : ");
    lcd.print(readLDR);
    break;
  default :
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pilihan Tampilan");
    lcd.setCursor(0, 1);
    lcd.print("1 to 9 or q to r");
    break;
  }
  if (Serial.available() > 0) 
  {
    sensor = Serial.read();
  }
  
  //SDCard
  File dataFile = SD.open("lala1.txt", FILE_WRITE);
                                  
  if (dataFile) 
  {
    dataFile.println(dataString);
    //dataFile.println(dataString2);
    dataFile.close();
                                      
    Serial.println(dataString);
    //Serial.println(dataString2);
  }
                                    
  else 
  {
    Serial.println("error opening uji.txt");
  }  
  delay(100);
}
