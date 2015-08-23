// Example program for Arduino 2WD robot
// The robot goes forward until an object is reconised.
// Stop and looks around, left and right and selects the bigger free direction

#include "Ultrasonic.h"
Ultrasonic ultrasonic(12,13); // init ultrasonic sensor pin 12=trigger, pin13=Echo

 #include <Servo.h>  // include servo functions
 Servo myservo;

// define Motor control pins
 
#define MotIN1  5 
#define MotIN2  6
#define MotIN3  10
#define MotIN4  9
#define MotApwm 11 //PWM output to set the motor speed
#define MotBpwm 3 //PWM output to set the motor speed
#define vac 7

// define direction 
#define GO  0
#define RIGHT 1
#define LEFT  2
#define BACK  3
#define STOP  4
#define  sMiddle 95
#define  sLeft 0
#define  sRight 165

// *** variables

int CMM;  //ultrasonic sensor distance values
int distL; //distance variable for left direction
int distR;
int SpeedA=200; // motor speed variable, 255=max
int SpeedB=195; //for some reson this needs to be lower to go straight


// Setup
void setup() {                
  // initialize the digital pin as an output.
  pinMode(MotIN1, OUTPUT);   
  pinMode(MotIN2, OUTPUT); 
  pinMode(MotIN3, OUTPUT); 
  pinMode(MotIN4, OUTPUT);
  pinMode(MotApwm, OUTPUT);
  pinMode(MotBpwm, OUTPUT);
  analogWrite(MotApwm, 200);  //set motor PWM to 200
  analogWrite(MotBpwm, 200);
  myservo.attach(10);  // servo control at pin 10
}

// ****** Function to set the motor driver lines ***
void setlines( int line1, int line2, int line3, int line4)   
{
  digitalWrite(MotIN1, line1);
  digitalWrite(MotIN2, line2);
  digitalWrite(MotIN3, line3);
  digitalWrite(MotIN4, line4);
}
//******* Function to set robot direction 

void robotdir(int dir)
{
switch(dir)
  {

case 0:    //GO
  setlines(HIGH, LOW, LOW, HIGH); 
  break;
case 1:    //Right
  setlines(LOW, HIGH, LOW, HIGH);
  break;
case 2:    //Left
   setlines(HIGH, LOW, HIGH, LOW); 
  break;
case 3:    //Back
   setlines(LOW, HIGH, HIGH, LOW); 
  break;
case 4:    //Stop
  setlines(LOW, LOW, LOW, LOW); 
  break;
 
  }
}
// **** Measuring distance at given position, and wait for servo setting
int MeasDistInPos(int servpos,int waitime)
{
  int result;
  myservo.write(servpos);
  delay(waitime); 
  result=ultrasonic.Ranging(CM);
  return result;
}

//************* TURN  ***********************
void Turn(int turndir)
{
  robotdir(turndir);
  analogWrite(MotApwm, 200); // the turning speed is lower
  analogWrite(MotBpwm, 200);
  delay(500);
  myservo.write(sMiddle);
  robotdir(STOP); 
  delay(500); 
}

//******************* MAIN LOOP **********************

// the loop routine runs over and over again forever:
void loop() 
{
  CMM = MeasDistInPos(sMiddle,50);
  if (CMM > 30)
  {
    robotdir(GO);
    analogWrite(MotApwm, 250); // tune  2 motors to run straight ahead
    analogWrite(MotBpwm, 250);  
  }  
  else
  {    
    analogWrite(MotApwm, 150);
    analogWrite(MotBpwm, 150);  
    robotdir(BACK); // brake with a short opposite direction
    delay(150);
    robotdir(STOP);
    Turn(LEFT);
    distR=MeasDistInPos(sMiddle,500); //look around left and right
    Turn(Right);
    distL=MeasDistInPos(sMiddle,500); 

   if (distL > distR) // select the bigger distance direction
   {
     Turn(LEFT);
   }
   else
   {
     Turn(RIGHT);   
   }
  }
}
