#include <TimerOne.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <arduino.h>
#include "motor.h"

//#define PI    3.14159265358
#define DTR   PI/180.0           // Konversi degree to radian
#define RTD   180.0/PI           // Konversi radian to degree

extern unsigned int speedL, speedR, pwmperiod;
extern long encL, encR, encLold, encRold;
extern float tetaL, tetaR;

int aprintf(char *str, ...)
{
  int i, j, count = 0;
  va_list argv;
  va_start(argv, str);
  for(i = 0, j = 0; str[i] != '\0'; i++) {
    if (str[i] == '%') {
      count++;

      Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);

      switch (str[++i]) {
        case 'd': Serial.print(va_arg(argv, int));
          break;
        case 'l': Serial.print(va_arg(argv, long));
          break;
        case 'f': Serial.print(va_arg(argv, double));
          break;
        case 'c': Serial.print((char) va_arg(argv, int));
          break;
        case 's': Serial.print(va_arg(argv, char *));
          break;
        case '%': Serial.print("%");
          break;
        default:;
      };

      j = i+1;
    }
  };
  va_end(argv);

  if(i > j) {
    Serial.write(reinterpret_cast<const uint8_t*>(str+j), i-j);
  }

  Serial.println(" ");
  delay(100);
  return count;
}

void movecase()
{
  char k;
  //   put your main code here, to run repeatedly:
  Serial.println("hehehe");
  if (Serial.available()>0) {
    switch(k=Serial.read()) {
      case '1':  speedR+=25; break;
      case '2':  speedR-=25; break;
      case '3':  speedL+=25; break;
      case '4':  speedL-=25; break;
      case '5':  pwmperiod+=50;motorinit(pwmperiod); break;
      case '6':  pwmperiod-=50;motorinit(pwmperiod); break;
      default:
        Serial.println(k);
    }
    motorRrun(speedR); 
    motorLrun(speedL); 
  }

  BacaEncR();
  BacaEncL();
  aprintf("%d: L:%d -encL:%l (%f), R:%d -encR:%l (%f)",pwmperiod, speedL,encL,tetaL, speedR,encR,tetaR);
}


#define sampling 1
bool enablePID = 0;
int samplingPID = 0;
float sudutA_ref = 0, sudutB_ref = 0;
float integralA = 0, derivativeA = 0, integralB = 0, derivativeB = 0, dt = 0.02*sampling;
float sinyal_kendaliA = 0, sinyal_kendaliB = 0;
float errorA = 0, errorB = 0;
float last_errorA = 0, last_errorB = 0;
float Kp = 5; //70
float Ki = 1; //0.1
float Kd = 1; //2

void PID()
{
  samplingPID++;
  if(samplingPID == sampling)
  {
    if(enablePID)
    {
      BacaEncR();
      BacaEncL();
      errorB = sudutB_ref + tetaL*360/4.63;
      errorA = sudutA_ref - tetaR;
      if(sinyal_kendaliA <= 255 && sinyal_kendaliA >= -255) integralA = integralA + errorA*dt;
      if(sinyal_kendaliB <= 255 && sinyal_kendaliB >= -255) integralB = integralB + errorB*dt;
      derivativeA = (errorA - last_errorA)/dt;
      derivativeB = (errorB - last_errorB)/dt;
      sinyal_kendaliA = Kp*errorA + Ki*integralA + Kd*derivativeA;
      sinyal_kendaliB = Kp*4*errorB + Ki*integralB + Kd*derivativeB;
      sinyal_kendaliA = constrain(sinyal_kendaliA,-1023,1023);
      sinyal_kendaliB = constrain(sinyal_kendaliB,-1023,1023);
      motorLrun(-1*sinyal_kendaliA);
      motorRrun(-1*sinyal_kendaliB);
      last_errorA = errorA;
      last_errorB = errorB;
    }
    samplingPID = 0;
  }
}

float fDC = 50;
float S_awalDC1 = 0;
float S_awalDC2 = 0;

void trajektori_DC(int t, float S_akhir)
{
  if(S_akhir != S_awalDC1 || S_akhir != S_awalDC2)
  {
    float sudut1, sudut2;
    for(int k = 0 ; k <= fDC*t ; k++)
    {
      sudut1 = S_awalDC1 + (S_akhir - S_awalDC1)*(float)k/fDC/t;
      sudut2 = S_awalDC2 + (S_akhir - S_awalDC2)*(float)k/fDC/t;
      sudutA_ref = sudut1;
      sudutB_ref = sudut2;
      delay(20);
    }
    Serial.print(-tetaL*360/4.63), Serial.print("\t"), Serial.println(tetaR);
    S_awalDC1 = sudut1;
    S_awalDC2 = sudut2;
  }
}

void trajektori_DC1(int t, float S_akhir1)
{
  if(S_akhir1 != S_awalDC1)
  {
    float sudut;
    for(int k = 0 ; k <= fDC*t ; k++)
    {
      sudut = S_awalDC1 + (S_akhir1 - S_awalDC1)*(float)k/fDC/t;
      sudutA_ref = sudut;
      delay(20);
    }
    Serial.print(-tetaL*360/4.63), Serial.print("\t"), Serial.println(tetaR);
    S_awalDC1 = sudut;
  }
}

void trajektori_DC2(int t, float S_akhir2)
{
  if(S_akhir2 != S_awalDC2)
  {
    float sudut;
    for(int k = 0 ; k <= fDC*t ; k++)
    {
      sudut = S_awalDC2 + (S_akhir2 - S_awalDC2)*(float)k/fDC/t;
      sudutB_ref = sudut;
      delay(20);
    }
    Serial.print(-tetaL*360/4.63), Serial.print("\t"), Serial.println(tetaR);
    S_awalDC2 = sudut;
  }
}

void demo_motorDC()
{
//  enablePID = 1;
//  float sudut=1080;
//  int waktu=5, motor;
//  trajektori_DC2(waktu,sudut);
  trajektori_DC(1,270);
  Serial.print(-tetaL*360/4.63), Serial.print("\t"), Serial.println(tetaR);
  delay(5000);
  trajektori_DC(1,0);
  Serial.print(-tetaL*360/4.63), Serial.print("\t"), Serial.println(tetaR);
  delay(5000);
  float sudut;
  int waktu, motor;
  Serial.println("Sudut : ");
  while(!Serial.available()){}
  if(Serial.available()) sudut = Serial.parseFloat();
  Serial.println("Waktu: ");
  while(!Serial.available()){}
  if(Serial.available()) waktu = Serial.parseInt();
  Serial.println("Motor:");
  Serial.println("1. Motor A");
  Serial.println("2. MotorB");
  Serial.println("3. Motor A dan B");
  while(!Serial.available()){}
  if(Serial.available()) motor = Serial.parseInt();
  if(motor==1) trajektori_DC1(waktu,sudut);
  else if(motor==2) trajektori_DC2(waktu,sudut);
  else if(motor==3) trajektori_DC(waktu,sudut);
}

void demo_serial()
{
  enablePID = 1;
  if(Serial.available()>=5)
  {
    unsigned char header = Serial.read();
    if(header == 0x41)
    {
      unsigned char low1 = Serial.read();
      unsigned char high1 = Serial.read();
      unsigned char low2 = Serial.read();
      unsigned char high2 = Serial.read();
      unsigned short sudutterima1 = (high1<<8) | low1;
      unsigned short sudutterima2 = (high2<<8) | low2;
      short sudut1 = sudutterima1 - 32767;
      short sudut2 = sudutterima2 - 32767;
      sudutA_ref = sudut1;
      sudutB_ref = sudut2;
    }
  }
}

int kkk = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  InitEncoder();
  Timer1.init(20000,8);
  Timer1.attachInterrupt(PID);
 
  // motor setup with fast pwm 1MHz 
  motorinit(pwmperiod); // assign PWM period in microsecond
  motorRinit();    // setup pinout for motor right
  motorLinit();    // setup pinout for motor left
  motorRrun(0);  // set positif pulse width of motor left/A, which should be less than PWM period
  motorLrun(0);  // set positif pulse width of motor right/B, which should be less than PWM period 
  delay(50);
}

void loop() {
//  for(kkk=0;kkk<1000;kkk++){
//    Serial.println(kkk);
//  }
  movecase();
//  demo_motorDC();
//demo_serial();
}
