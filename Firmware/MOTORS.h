/* This file contains the functions and backing variables required to move the 
 * Key reletive to the cutting wheel. The update functions should be called
 * during each mainloop cycle when genesis is operating. The three positional
 * update functions (X, Y, C) will move the axis towards there 
 * 
 * Author:        Jake Ketchum
 * Last Changed:  12/20/16
 */

// Motor Data:

#include <time.h>;
#include "PINS.h"

int  CurrentX = 0;    // Current location is stored as an offset (in steps) from 
int  CurrentZ = 0;    // The zero point (touch both limit switches with C at 180
int  CurrentC = 0;    // degrees, fully virtical).

long  TargetX  = 0;    // The location to which each of the axis is moving. Stored
long  TargetZ  = 800;    // as an offset "in steps" from the zero point of that axis. 
long  TargetC  = 0;    // 

long  TimerX = 0;
long  TimerZ = 0;
long  TimerC = 0;

int  TimeX = 10;
int  TimeZ = 10;
int  TimeC = 10;

int  localMillis = 0;
int  SpeedS = 0;     // cutter speed as a percentage.

void updateTimers(){
   
   int difference = millis() - localMillis;
   localMillis = millis();
   TimerX += difference;
   TimerZ += difference;
   TimerC += difference;
}


boolean  updateX(){
  /* Responcible for moving the X axis stepper one step if appropriate.
   * Returns: True if at location, false otherwise. 
   */
  // check if target location reached.  
  if (TargetX == CurrentZ){
    TimerX = 0;
    return false; 
  }

  // not at target, check if time for pulse. 
  if (TimerX < TimeZ){
    return false;
  }

  // Time for pulse, set direction.
  if (TargetX > CurrentX){
    digitalWrite(PIN_Z_DIRECTION, HIGH);
    CurrentX += 1; // incriment location variable. 
  } else {
    digitalWrite(PIN_Z_DIRECTION, LOW);
    CurrentX -= 1; // decriment location variable. 
  }

  // It's time for a pulse.
  digitalWrite(PIN_X_PULSE, LOW);
  delayMicroseconds(1); 
  digitalWrite(PIN_X_PULSE, HIGH);
  // update timer variable. 
  TimerX -= TimeX;
  return true;
}
  
}

boolean  updateZ(){
  
   /* Responcible for moving the Z axis stepper one step if appropriate.
   * Returns: True if at location, false otherwise. 
   */
  // check if target location reached.  
  if (TargetZ == CurrentZ){
    TimerZ = 0;
    return false; 
  }

  // not at target, check if time for pulse. 
  if (TimerZ < TimeZ){
    return false;
  }

  // Time for pulse, set direction.
  if (TargetZ > CurrentZ){
    digitalWrite(PIN_Z_DIRECTION, HIGH);
    CurrentZ += 1; // incriment location variable. 
  } else {
    digitalWrite(PIN_Z_DIRECTION, LOW);
    CurrentZ -= 1; // decriment location variable. 
  }

  // It's time for a pulse.
  digitalWrite(PIN_Z_PULSE, LOW);
  delayMicroseconds(1); 
  digitalWrite(PIN_Z_PULSE, HIGH);
  // update timer variable. 
  TimerZ -= TimeZ;
  return true;
}

boolean  updateC(){


  /* Responcible for moving the C axis stepper one step if appropriate.
   * Returns: True if at location, false otherwise. 
   */
  // check if target location reached.  
  if (TargetC == CurrentC){
    TimerC = 0;
    return false; 
  }

  // not at target, check if time for pulse. 
  if (TimerC < TimeC){
    return false;
  }

  // Time for pulse, set direction.
  if (TargetC > CurrentC){
    digitalWrite(PIN_C_DIRECTION, HIGH);
    CurrentZ += 1; // incriment location variable. 
  } else {
    digitalWrite(PIN_C_DIRECTION, LOW);
    CurrentC -= 1; // decriment location variable. 
  }

  // It's time for a pulse.
  digitalWrite(PIN_C_PULSE, LOW);
  delayMicroseconds(1); 
  digitalWrite(PIN_C_PULSE, HIGH);
  // update timer variable. 
  TimerC -= TimeC;
  return true;
}
}

boolean  updateS(){
  
}

boolean  updateMotors(){
  updateTimers();
  
  boolean X = updateX();
  boolean Z = updateZ();
  boolean C = updateC();
  boolean S = updateS();
}

