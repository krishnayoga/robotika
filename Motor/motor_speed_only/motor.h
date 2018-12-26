// motor mechanic
#define PPR_MOTOR (128.0/*PPR*/*27.0/*Gear ratio*/*1.0/*quadrature*/)
#define RESOLUTION_D 360.0/PPR_MOTOR
#define RESOLUTION_R 2.0*PI/PPR_MOTOR
#define RADIUS_BAN (0.03) // dalam meter
#define ROBOT_TREAD (0.17) // dalam meter (jarak antara roda)

// circuit mapping
// motor A channel as right side
// motor B channel as left side
// Encoder X channel as right side
// Encoder Y channel as left side

//motor
#define  ML1 22
#define  ML2 24
#define  MR1 26
#define  MR2 28
#define  MREN 5
#define  MLEN 3
#define  MOTORCLK 10
#define  ENABLEPWM 1
#define  ENABLEONLY 2

#define  MAXOCREN 1023
#define  MAXOCRIN 511

#define  MLdir -1.0
#define  MRdir 1.0

#define  ELdir 1
#define  ERdir -1

//encoder
#define HCTL2032_PIN_OE   8
#define HCTL2032_PIN_XY   34
#define HCTL2032_PIN_EN1   13
#define HCTL2032_PIN_EN2   12
#define HCTL2032_PIN_SEL1 11
#define HCTL2032_PIN_SEL2 36
#define HCTL2032_PIN_RSTX  6
#define HCTL2032_PIN_RSTY  7

#define HCTL2032_PIN_D0 A8
#define HCTL2032_PIN_D1 A9
#define HCTL2032_PIN_D2 A10
#define HCTL2032_PIN_D3 A11
#define HCTL2032_PIN_D4 A12
#define HCTL2032_PIN_D5 A13
#define HCTL2032_PIN_D6 A14
#define HCTL2032_PIN_D7 A15


//definisikan function prototype
void InitEncoder(void);
void BacaEncL(void);
void BacaEncR(void);
void ResetEncL(void);
void ResetEncR(void);
void ModeState(unsigned char mode);
void gen_clock1MHz(void);

void motorinit(unsigned int maxspeed); // maksimum 2byte 65535
void motorRinit(void);
void motorLinit(void);
void motorRrun(float speedf);
void motorLrun(float speedf);
