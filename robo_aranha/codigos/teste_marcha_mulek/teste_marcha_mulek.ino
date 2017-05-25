// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

// Posiçoes do do Mulek
// A  B          A B  
//        _____
// 10 11 |     | 0 1
//        |   |
// 8  9  |     | 2 3
//        |   |
// 6  7  |_____| 4 5

// Posiçoes do Pequeno Aranha
// A  B           A  B  
//        _____
// 1  0  |     |  7  6
//        |   |
// 3  2  |     |  9  8
//        |   |
// 5  4  |_____| 11 10

#include <Servo.h> 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
 
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
 
 
Servo gammaServo[13];  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 


 
int gammaOrigem[13];    // variable to store the servo position 
int numMaxServo = 6;

int alphaBetaOrigem[13];
int servoPos[12]; 
 
int estados [49][12] = 
{{ -10 , -50 , -2 , -50 , -6 , -50 , 4 , 50 , 8 , 50 , 0 , 50 }, 
{ -8 , -45 , -2 , -50 , -6 , -50 , 4 , 50 , 8 , 50 , 0 , 50 }, 
{ -7 , -40 , -2 , -50 , -6 , -50 , 4 , 50 , 8 , 50 , 0 , 50 }, 
{ -6 , -35 , -2 , -50 , -6 , -50 , 4 , 50 , 8 , 50 , 0 , 50 }, 
{ -5 , -30 , -3 , -50 , -7 , -50 , 5 , 50 , 9 , 50 , 1 , 50 }, 
{ -3 , -35 , -3 , -50 , -7 , -50 , 5 , 50 , 9 , 50 , 1 , 50 }, 
{ -2 , -40 , -3 , -50 , -7 , -50 , 5 , 50 , 9 , 50 , 1 , 50 }, 
{ -1 , -45 , -3 , -50 , -7 , -50 , 5 , 50 , 9 , 50 , 1 , 50 }, 
{ 0 , -50 , -4 , -50 , -8 , -50 , 6 , 50 , 10 , 50 , 2 , 50 }, 
{ 0 , -50 , -4 , -50 , -8 , -50 , 6 , 50 , 8 , 45 , 2 , 50 }, 
{ 0 , -50 , -4 , -50 , -8 , -50 , 6 , 50 , 7 , 40 , 2 , 50 }, 
{ 0 , -50 , -4 , -50 , -8 , -50 , 6 , 50 , 6 , 35 , 2 , 50 }, 
{ -1 , -50 , -5 , -50 , -9 , -50 , 7 , 50 , 5 , 30 , 3 , 50 }, 
{ -1 , -50 , -5 , -50 , -9 , -50 , 7 , 50 , 3 , 35 , 3 , 50 }, 
{ -1 , -50 , -5 , -50 , -9 , -50 , 7 , 50 , 2 , 40 , 3 , 50 }, 
{ -1 , -50 , -5 , -50 , -9 , -50 , 7 , 50 , 1 , 45 , 3 , 50 }, 
{ -2 , -50 , -6 , -50 , -10 , -50 , 8 , 50 , 0 , 50 , 4 , 50 }, 
{ -2 , -50 , -6 , -50 , -8 , -45 , 8 , 50 , 0 , 50 , 4 , 50 }, 
{ -2 , -50 , -6 , -50 , -7 , -40 , 8 , 50 , 0 , 50 , 4 , 50 }, 
{ -2 , -50 , -6 , -50 , -6 , -35 , 8 , 50 , 0 , 50 , 4 , 50 }, 
{ -3 , -50 , -7 , -50 , -5 , -30 , 9 , 50 , 1 , 50 , 5 , 50 }, 
{ -3 , -50 , -7 , -50 , -3 , -35 , 9 , 50 , 1 , 50 , 5 , 50 }, 
{ -3 , -50 , -7 , -50 , -2 , -40 , 9 , 50 , 1 , 50 , 5 , 50 }, 
{ -3 , -50 , -7 , -50 , -1 , -45 , 9 , 50 , 1 , 50 , 5 , 50 }, 
{ -4 , -50 , -8 , -50 , 0 , -50 , 10 , 50 , 2 , 50 , 6 , 50 }, 
{ -4 , -50 , -8 , -50 , 0 , -50 , 8 , 45 , 2 , 50 , 6 , 50 }, 
{ -4 , -50 , -8 , -50 , 0 , -50 , 7 , 40 , 2 , 50 , 6 , 50 }, 
{ -4 , -50 , -8 , -50 , 0 , -50 , 6 , 35 , 2 , 50 , 6 , 50 }, 
{ -5 , -50 , -9 , -50 , -1 , -50 , 5 , 30 , 3 , 50 , 7 , 50 }, 
{ -5 , -50 , -9 , -50 , -1 , -50 , 3 , 35 , 3 , 50 , 7 , 50 }, 
{ -5 , -50 , -9 , -50 , -1 , -50 , 2 , 40 , 3 , 50 , 7 , 50 }, 
{ -5 , -50 , -9 , -50 , -1 , -50 , 1 , 45 , 3 , 50 , 7 , 50 }, 
{ -6 , -50 , -10 , -50 , -2 , -50 , 0 , 50 , 4 , 50 , 8 , 50 }, 
{ -6 , -50 , -8 , -45 , -2 , -50 , 0 , 50 , 4 , 50 , 8 , 50 }, 
{ -6 , -50 , -7 , -40 , -2 , -50 , 0 , 50 , 4 , 50 , 8 , 50 }, 
{ -6 , -50 , -6 , -35 , -2 , -50 , 0 , 50 , 4 , 50 , 8 , 50 }, 
{ -7 , -50 , -5 , -30 , -3 , -50 , 1 , 50 , 5 , 50 , 9 , 50 }, 
{ -7 , -50 , -3 , -35 , -3 , -50 , 1 , 50 , 5 , 50 , 9 , 50 }, 
{ -7 , -50 , -2 , -40 , -3 , -50 , 1 , 50 , 5 , 50 , 9 , 50 }, 
{ -7 , -50 , -1 , -45 , -3 , -50 , 1 , 50 , 5 , 50 , 9 , 50 }, 
{ -8 , -50 , 0 , -50 , -4 , -50 , 2 , 50 , 6 , 50 , 10 , 50 }, 
{ -8 , -50 , 0 , -50 , -4 , -50 , 2 , 50 , 6 , 50 , 8 , 45 }, 
{ -8 , -50 , 0 , -50 , -4 , -50 , 2 , 50 , 6 , 50 , 7 , 40 }, 
{ -8 , -50 , 0 , -50 , -4 , -50 , 2 , 50 , 6 , 50 , 6 , 35 }, 
{ -9 , -50 , -1 , -50 , -5 , -50 , 3 , 50 , 7 , 50 , 5 , 30 }, 
{ -9 , -50 , -1 , -50 , -5 , -50 , 3 , 50 , 7 , 50 , 3 , 35 }, 
{ -9 , -50 , -1 , -50 , -5 , -50 , 3 , 50 , 7 , 50 , 2 , 40 }, 
{ -9 , -50 , -1 , -50 , -5 , -50 , 3 , 50 , 7 , 50 , 1 , 45 }, 
{ -10 , -50 , -2 , -50 , -6 , -50 , 4 , 50 , 8 , 50 , 0 , 50 }};
 
void setup() 
{
  servoPos[0] = 11;
  servoPos[1] = 10; 
  servoPos[2] = 9;
  servoPos[3] = 8; 
  servoPos[4] = 7;
  servoPos[5] = 6; 
  servoPos[6] = 1;
  servoPos[7] = 0; 
  servoPos[8] = 3;
  servoPos[9] = 2; 
  servoPos[10] = 5;
  servoPos[11] = 4; 
 
 
  gammaServo[0].attach(3);
  gammaServo[1].attach(5);
  gammaServo[2].attach(6);
  gammaServo[3].attach(9);
  gammaServo[4].attach(10);
  gammaServo[5].attach(11); 
  
  gammaOrigem[0] = 88;
  gammaOrigem[1] = 90;
  gammaOrigem[2] = 75;
  gammaOrigem[3] = 110;
  gammaOrigem[4] = 110;
  gammaOrigem[5] = 83;  

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
} 
 
void setMotorAngle(uint8_t num, uint16_t degree){
   uint16_t pulselen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
   pwm.setPWM(num, 0, pulselen);
  
}
 
bool emPe = false; 
 
void loop() 
{  
  
  /*
  if (emPe) {
   
   
  } 
  */
  
  
  int degree = 90;
  int alphaSet = 90;
  int betaSet = 30;
  
  uint16_t gammaSet = -40; 
  for (int i = 0; i < 6; i++) {
     if ( i >= 3 ) 
       gammaSet = 40;
     gammaServo[i].write(gammaOrigem[i] + gammaSet);
  }   

  for (int j=0; j < 49; j++) { // j identifica o estado 
    for (int i=0; i < 12; i++) {
      //setMotorAngle(servoPos[i],degree + estados[j][i]);
      if ( i < 6 )
        betaSet =  90;
      else
        betaSet = -90;
      if ( (i % 2) == 0 ) 
        setMotorAngle(i,degree + estados[j][i]);
      else 
        setMotorAngle(i,degree + betaSet + estados[j][i]);
      delay(5);
    } 
   //   myservo[i].write(pos[i] + estados[j][i]);              // tell servo to go to position in variable 'pos' 
    delay(100);                       // waits 15ms for the servo to reach the position  
  }
  
}
