#include <dht.h>
#include <Wire.h>

#define DHT_pin 2
#define GP_sensor 2
#define LDR_pin 1
#define PIR_pin 4

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C
 
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18
 
#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define ADDRESS 0x70

dht DHT;

int photoDiode = 2;                                       
int sensorRead = 3;                  
int sensor_thr = 905;
float temp_lm35;
int tempPin = 0;

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(GP_sensor, INPUT);
  pinMode(photoDiode,INPUT);  
  digitalWrite(photoDiode,HIGH);
  pinMode(LDR_pin,  INPUT);
  pinMode(PIR_pin, INPUT);

  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
  I2CwriteByte(MAG_ADDRESS, 0x0A, 0x01);

  Serial.print("Hum");
  Serial.print("\t");
  Serial.print("Temp");
  Serial.print("\t");
  Serial.print("GP");
  Serial.print("\t");
  Serial.print("val");
  Serial.print("\t");
  Serial.print("LDR");
  Serial.print("\t");
  Serial.print("temp_lm35");
  Serial.print("\t");
  Serial.print("ax");
  Serial.print("\t");
  Serial.print("ay");
  Serial.print("\t");
  Serial.print("az");
  Serial.print("\t");
  Serial.print("gx");
  Serial.print("\t");
  Serial.print("gy");
  Serial.print("\t");
  Serial.print("gz");
  Serial.print("\t");
  Serial.print("mx");
  Serial.print("\t");
  Serial.print("my");
  Serial.print("\t");
  Serial.print("mz");
  Serial.print("\t");
  Serial.print("PIR");
  Serial.print("\t");
  Serial.print("SRF");
  Serial.print("\t");
}

void loop() {
  
  int chk = DHT.read22(DHT_pin);
  int GP = analogRead(GP_sensor);
  int val = analogRead(sensorRead);
  int LDR = analogRead(LDR_pin);
  temp_lm35 = analogRead(tempPin);
  temp_lm35 = temp_lm35 * 0.48828125;

  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
 
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];
 
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = Buf[12] << 8 | Buf[13];

  uint8_t ST1;

  I2Cread(MAG_ADDRESS, 0x02, 1, &ST1);

  uint8_t Mag[7];
  I2Cread(MAG_ADDRESS, 0x03, 7, Mag);
 
  int16_t mx = -(Mag[3] << 8 | Mag[2]);
  int16_t my = -(Mag[1] << 8 | Mag[0]);
  int16_t mz = -(Mag[5] << 8 | Mag[4]);

  byte byteHigh, byteLow;
  int distance;
  Wire.beginTransmission(ADDRESS); 
  Wire.write(byte(0x00));         
  Wire.write(byte(0x51));         
  Wire.endTransmission();     
  delay(10);
  Wire.beginTransmission(ADDRESS); 
  Wire.write(byte(0x02));        
  Wire.endTransmission();       
  Wire.requestFrom(ADDRESS, 2); 
  while(Wire.available() < 2);
    byteHigh = Wire.read(); 
    byteLow = Wire.read();
  distance = ((byteHigh<<8) + byteLow);
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.print(DHT.temperature,1);
  Serial.print(",\t");
  Serial.println(GP);
  Serial.print("\t");
  Serial.print(val);
  Serial.print("\t");
  Serial.print(LDR);
  Serial.print("\t");
  Serial.print(temp_lm35);
  Serial.print("\t");

  Serial.print(ax, DEC);
  Serial.print("\t");
  Serial.print(ay, DEC);
  Serial.print("\t");
  Serial.print(az, DEC);
  Serial.print("\t");
 
  Serial.print(gx, DEC);
  Serial.print("\t");
  Serial.print(gy, DEC);
  Serial.print("\t");
  Serial.print(gz, DEC);
  Serial.print("\t");
 
  Serial.print(mx + 200, DEC);
  Serial.print("\t");
  Serial.print(my - 70, DEC);
  Serial.print("\t");
  Serial.print(mz - 700, DEC);
  Serial.print("\t");

  if(detectOrang()){
    Serial.print("Ada Orang");
    Serial.print("\t");
  }
  else{
    Serial.print("Tidak Ada Orang");
    Serial.print("\t");
  }
  Serial.print(distance, DEC);
  Serial.print("\t");
  
  Serial.println();
}

boolean detectOrang(){
  
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
