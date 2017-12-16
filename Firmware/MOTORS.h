/* This file contains the functions and backing variables required to move the 
 * Key reletive to the cutting wheel. The update functions should be called
 * during each mainloop cycle when genesis is operating. The three positional
 * update functions (X, Y, C) will move the axis towards there 
 * 
 * Author:        Jake Ketchum
 * Last Changed:  12/20/16
 */

// Motor Data:
#include <Servo.h>
#include <time.h>;
#include "PINS.h"

long  CurrentX = 0;    // Current location is stored as an offset (in steps) from 
long  CurrentZ = 0;    // The zero point (touch both limit switches with C at 180
long  CurrentC = 0;    // degrees, fully virtical).

long  TargetX  = 0;    // The location to which each of the axis is moving. Stored
long  TargetZ  = 0;    // as an offset "in steps" from the zero point of that axis. 
long  TargetC  = 0;    // 

long  TimerX = 0;
long  TimerZ = 0;
long  TimerC = 0;

int  TimeX = 10;
int  TimeZ = 10;
int  TimeC = 10;

long Current[] = {0,0,0};
long Target[]  = {0,0,0};
long Timer[]   = {0,0,0};
int  Time[]    = {0,0,0};
bool reverse[] = {REVERSE_X, REVERSE_Z, REVERSE_C};


int  localMicros = 0;
int  SpeedS = 0;     // cutter speed as a percentage.
Servo spindle;
  
void updateTimers(){
   
   int difference = micros() - localMicros;
   localMicros = micros();
   Timer[XIN] += difference;
   Timer[ZIN] += difference;
   Timer[CIN] += difference;
}

boolean update_stepper(int i, int Pinpulse, int pinDirection){
   /* Responcible for moving the stepper motor specified by one step if appropriate.
   * Returns: True if at location, false otherwise. 
   */
  // check if target location reached.  
  if (Target[i] == Current[i]){
    Timer[i] = 0;
    return false; 
  }

  // not at target, check if time for pulse. 
  if (Timer[i] < Time[i]){
    return false;
  }

  // Time for pulse, set direction.
  if (Target[i] > Current[i]){
    digitalWrite(pinDirection, HIGH != reverse[i]);
    Current[i] += 1; // incriment location variable. 
  } else {
    digitalWrite(pinDirection, LOW != reverse[i]);
    Current[i] -= 1; // decriment location variable. 
  }

  // It's time for a pulse.
  digitalWrite(Pinpulse, LOW);
  delayMicroseconds(1); 
  digitalWrite(Pinpulse, HIGH);
  delayMicroseconds(1); 
  // update timer variable. 
  Timer[i] -= Time[i];
  return true;
}

boolean  updateX(){
  /* Responcible for moving the X axis stepper one step if appropriate.
   * Returns: True if at location, false otherwise. 
   */
  return(update_stepper(XIN, PIN_X_PULSE, PIN_X_DIRECTION));
}

boolean  updateZ(){
  return(update_stepper(ZIN, PIN_Z_PULSE, PIN_Z_DIRECTION));
}

boolean  updateC(){
  return(update_stepper(CIN, PIN_C_PULSE, PIN_C_DIRECTION));
}

boolean  updateS(){
  int spindleSpeed = SpeedS;
  spindleSpeed = map(spindleSpeed, 0, 100, 100, 2000);
  spindle.writeMicroseconds(spindleSpeed);
  return true;
}

boolean  updateMotors(){
  updateTimers();
  boolean X = updateX();
  boolean Z = updateZ();
  boolean C = updateC();
  boolean S = updateS();
}

