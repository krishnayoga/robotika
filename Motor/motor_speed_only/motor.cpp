#include <arduino.h>
#include <TimerThree.h>

#include "motor.h"

unsigned int speedL=0, speedR=0, pwmperiod=1000;
long encL = 0, encR = 0, encLold = 0, encRold = 0;
float tetaL=0, tetaR=0;
int drivermode = ENABLEONLY;

void motorinit(unsigned int maxspeed)
{
  Timer3.initialize(maxspeed);  // 40 us = 25 kHz
  //DDRE=0x28;

  // Timer/Counter 3 initialization
  // Clock source: System Clock
  // Clock value: 2000.000 kHz
  // Mode: Fast PWM top=ICR3
  // OC3A output: Non-Inv.
  // OC3B output: Discon.
  // OC3C output: Non-Inv.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // Timer3 Overflow Interrupt: Off
  // Input Capture Interrupt: Off
  // Compare A Match Interrupt: Off
  // Compare B Match Interrupt: Off
  // Compare C Match Interrupt: Off
  //TCCR3A=0x8A;
  //TCCR3B=0x1A;
  //TCNT3H=0x00;
  //TCNT3L=0x00;
  //ICR3H=0x01;
  //ICR3L=0x00;
  //OCR3AH=0x00;
  //OCR3AL=0x50;
  //OCR3BH=0x00;
  //OCR3BL=0x00;
  //OCR3CH=0x00;
  //OCR3CL=0x50;
}

void motorRinit(void)
{
  pinMode( MR1, OUTPUT);
  pinMode( MR2, OUTPUT);
  pinMode( MREN, OUTPUT);
}
void motorLinit(void)
{

  pinMode( ML1, OUTPUT);
  pinMode( ML2, OUTPUT);
  pinMode( MLEN, OUTPUT);
}
void motorRrun(float speedf)
{
  int pwm = (int) speedf*MRdir;
  unsigned int speedocr;
  
  if (drivermode==ENABLEPWM) { // jumper di deket hctl
     if (pwm > 0) {
        digitalWrite(MR1, HIGH);
        digitalWrite(MR2, LOW);
     } else {
        digitalWrite(MR1, LOW);
        digitalWrite(MR2, HIGH);
     }
     speedocr = abs(pwm);
     if (speedocr>1023) speedocr=1023;
  } else { // jumper di deket socket motor
     digitalWrite(MR1, HIGH);
     if (pwm>MAXOCRIN) pwm=MAXOCRIN; 
     if (pwm<-MAXOCRIN) pwm=-MAXOCRIN;
     speedocr = 512 + pwm ;
  }
  Timer3.pwm(MREN, speedocr);  // hanya menggunakan scala 0-1023
  //  OCR3AH=speedocr >> 8;
  //  OCR3AL=speedocr & 0xFF;
}
void motorLrun(float speedf)
{
  int pwm = (int) speedf*MLdir;
  unsigned int speedocr;;
  
  if (drivermode==ENABLEPWM) { // jumper di deket hctl
     if (pwm > 0) {
       digitalWrite(ML1, HIGH);
       digitalWrite(ML2, LOW);
     } else {
       digitalWrite(ML1, LOW);
       digitalWrite(ML2, HIGH);
     }
     speedocr = abs(pwm);
     if (speedocr>1023) speedocr=1023;
  } else { // jumper di deket socket motor
     digitalWrite(ML1, HIGH);
     if (pwm>MAXOCRIN) pwm=MAXOCRIN; 
     if (pwm<-MAXOCRIN) pwm=-MAXOCRIN;
     speedocr = 512 + pwm ;
  }
  Timer3.pwm(MLEN, speedocr);  // hanya menggunakan scala 0-1023
  
  //  OCR3CH=speedocr >> 8;
  //  OCR3CL=speedocr & 0xFF;
}

void InitEncoder(void) {
  pinMode (MOTORCLK, OUTPUT);
  digitalWrite(MOTORCLK, HIGH);
  //dipanggil di setup
  pinMode(HCTL2032_PIN_XY, OUTPUT);
  pinMode(HCTL2032_PIN_OE, OUTPUT);
  pinMode(HCTL2032_PIN_EN1, OUTPUT);
  pinMode(HCTL2032_PIN_EN2, OUTPUT);
  pinMode(HCTL2032_PIN_SEL1, OUTPUT);
  pinMode(HCTL2032_PIN_SEL2, OUTPUT);
  pinMode(HCTL2032_PIN_RSTX, OUTPUT);
  pinMode(HCTL2032_PIN_RSTY, OUTPUT);
  DDRK = 0x00;

  // XY LOW  X Axis AKA 1st Axis
  // XY HIGH Y Axis AKA 2nd Axis
  digitalWrite(HCTL2032_PIN_XY, LOW);
  digitalWrite(HCTL2032_PIN_OE, HIGH);  // Active LOW

  ModeState(1); //four time reading

  // Byte Selected MSB SEL1  LOW SEL2 HIGH
  // Byte Selected 2nd SEL1 HIGH SEL2 HIGH
  // Byte Selected 3rd SEL1  LOW SEL2 LOW
  // Byte Selected LSB SEL1 HIGH SEL2 LOW
  digitalWrite(HCTL2032_PIN_SEL1, LOW);
  digitalWrite(HCTL2032_PIN_SEL2, HIGH);

  digitalWrite(HCTL2032_PIN_RSTX, HIGH);  // Active LOW
  digitalWrite(HCTL2032_PIN_RSTY, HIGH);  // Active LOW

  ResetEncR();
  ResetEncL();

}

void BacaEncR(void) {
  unsigned long DataEnc1 = 0;
  unsigned long DataEnc11 = 0;
  unsigned char DataTemp1;
  //Baca MSB
  digitalWrite(HCTL2032_PIN_XY, LOW);
  digitalWrite(HCTL2032_PIN_SEL1, LOW);
  digitalWrite(HCTL2032_PIN_SEL2, HIGH);
  digitalWrite(HCTL2032_PIN_OE, LOW);
  delayMicroseconds(5);
  DataTemp1 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc1 = DataTemp1;
  DataEnc1 <<= 8;
  //DataEnc1 = DataEnc1 | ((unsigned long)DataTemp1 << 24);

  //Baca 2nd
  digitalWrite(HCTL2032_PIN_SEL1, HIGH);
  digitalWrite(HCTL2032_PIN_SEL2, HIGH);
  delayMicroseconds(5);
  DataTemp1 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc1 += DataTemp1;
  DataEnc1 <<= 8;
  //  DataEnc1 = DataEnc1 | ((unsigned long)DataTemp1 << 16);
  DataEnc11 = DataTemp1;

  //Baca 3rd
  digitalWrite(HCTL2032_PIN_SEL1, LOW);
  digitalWrite(HCTL2032_PIN_SEL2, LOW);
  delayMicroseconds(5);
  DataTemp1 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc1 += DataTemp1;
  DataEnc1 <<= 8;
  //  DataEnc1 = DataEnc1 | ((unsigned long)DataTemp1 << 8);

  //Baca LSB
  digitalWrite(HCTL2032_PIN_SEL1, HIGH);
  digitalWrite(HCTL2032_PIN_SEL2, LOW);
  delayMicroseconds(5);
  DataTemp1 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc1 += DataTemp1;
  //  DataEnc1 = DataEnc1 | ((unsigned long)DataTemp1);


  digitalWrite(HCTL2032_PIN_OE, HIGH);
  encR = (long)DataEnc1 * ERdir;
  tetaR += (float)(encR - encRold) * RESOLUTION_D;
  encRold = encR;
}

void BacaEncL(void) {
  unsigned long DataEnc2 = 0;
  unsigned long DataEnc22 = 0;
  unsigned char DataTemp2;
  //Baca MSB
  digitalWrite(HCTL2032_PIN_XY, HIGH);
  digitalWrite(HCTL2032_PIN_SEL1, LOW);
  digitalWrite(HCTL2032_PIN_SEL2, HIGH);
  digitalWrite(HCTL2032_PIN_OE, LOW);
  delayMicroseconds(5);
  DataTemp2 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc2 += DataTemp2;
  DataEnc2 <<= 8;
  //  DataEnc2 = DataEnc2 | ((unsigned long)DataTemp2 << 24);

  //Baca 2nd
  digitalWrite(HCTL2032_PIN_SEL1, HIGH);
  digitalWrite(HCTL2032_PIN_SEL2, HIGH);
  delayMicroseconds(5);
  DataTemp2 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc2 += DataTemp2;
  DataEnc2 <<= 8;
  DataEnc22 = DataTemp2;
  //  DataEnc2 = DataEnc2 | ((unsigned long)DataTemp2 << 16);

  //Baca 3rd
  digitalWrite(HCTL2032_PIN_SEL1, LOW);
  digitalWrite(HCTL2032_PIN_SEL2, LOW);
  delayMicroseconds(5);
  DataTemp2 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc2 += DataTemp2;
  DataEnc2 <<= 8;
  //  DataEnc2 = DataEnc2 | ((unsigned long)DataTemp2 << 8);

  //Baca LSB
  digitalWrite(HCTL2032_PIN_SEL1, HIGH);
  digitalWrite(HCTL2032_PIN_SEL2, LOW);
  delayMicroseconds(5);
  DataTemp2 = PINK; //definisikan register I/O 8 bit untuk data encoder
  DataEnc2 += DataTemp2;
  //  DataEnc2 = DataEnc2 | ((unsigned long)DataTemp2);

  digitalWrite(HCTL2032_PIN_OE, HIGH);
  encL = (long)DataEnc2 * ELdir;
  tetaL += (float)(encL - encLold) / 100.0;
  encLold = encL;
}

void ResetEncR(void) {
  digitalWrite(HCTL2032_PIN_RSTX, LOW);
  delayMicroseconds(1);
  digitalWrite(HCTL2032_PIN_RSTX, HIGH);
  delayMicroseconds(1);
}

void ResetEncL(void) {
  digitalWrite(HCTL2032_PIN_RSTY, LOW);
  delayMicroseconds(1);
  digitalWrite(HCTL2032_PIN_RSTY, HIGH);
  delayMicroseconds(1);
}

void ModeState(unsigned char mode)
{
  // pilih mode bisa dilihat langsung dari datasheet
  switch (mode)
  {
    case 1: // 1X Count Mode
      digitalWrite(HCTL2032_PIN_EN1, HIGH);
      digitalWrite(HCTL2032_PIN_EN2, HIGH);
      break;
    case 2: // 2X Count Mode
      digitalWrite(HCTL2032_PIN_EN1, LOW);
      digitalWrite(HCTL2032_PIN_EN2, HIGH);
      break;
    case 4: // 4X Count Mode is the default
    default:
      digitalWrite(HCTL2032_PIN_EN1, HIGH);
      digitalWrite(HCTL2032_PIN_EN2, LOW);
      break;
  }
  delayMicroseconds(1);
}
