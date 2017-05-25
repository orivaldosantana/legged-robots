// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo[13];  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos[13];    // variable to store the servo position 
int numMaxServo = 12;
 
int estados [9][12] = 
{{ -45 , -30 , 0 , -30 , -45 , -30 , 0 , 30 , 45 , 30 , 0 , 30 }, 
{ -33 , -5 , -11 , -30 , -33 , -5 , 11 , 30 , 33 , 5 , 11 , 30 }, 
{ -22 , 20 , -22 , -30 , -22 , 20 , 22 , 30 , 22 , -20 , 22 , 30 }, 
{ -11 , -5 , -33 , -30 , -11 , -5 , 33 , 30 , 11 , 5 , 33 , 30 }, 
{ 0 , -30 , -45 , -30 , 0 , -30 , 45 , 30 , 0 , 30 , 45 , 30 }, 
{ -11 , -30 , -33 , -5 , -11 , -30 , 33 , 5 , 11 , 30 , 33 , 5 }, 
{ -22 , -30 , -22 , 20 , -22 , -30 , 22 , -20 , 22 , 30 , 22 , -20 }, 
{ -33 , -30 , -11 , -5 , -33 , -30 , 11 , 5 , 33 , 30 , 11 , 5 }, 
{ -45 , -30 , 0 , -30 , -45 , -30 , 0 , 30 , 45 , 30 , 0 , 30 } };
 
void setup() 
{ 
  pos[0] = 75;
  pos[1] = 115;
  pos[2] = 90;
  pos[3] = 100;
  pos[4] = 90;
  pos[5] = 95;
  pos[6] = 78;
  pos[7] = 84;  
  pos[8] = 90;
  pos[9] = 114; 
  pos[10] = 91;
  pos[11] = 94; 
  for (int i=0; i < numMaxServo; i++)
    myservo[i].attach(i+2);  // attaches the servo on pin i+2 to the servo object 
    // i = 0 -> 2
    // i = 1 -> 3 
} 
 
 
void loop() 
{  
  for (int j=0; j < 9; j++) { // j identifica o estado 
    for (int i=0; i < numMaxServo; i++)
      myservo[i].write(pos[i] + estados[j][i]);              // tell servo to go to position in variable 'pos' 
    delay(150);                       // waits 15ms for the servo to reach the position  
  }
  
}
