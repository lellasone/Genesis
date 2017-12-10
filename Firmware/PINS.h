 /* 
 *  This file contains the pin configuration of the arduino as well as 
 *  general operational constants and the command strings. It should be 
 *  updated when first bringing up a new Genesis unit. For the most part 
 *  arduino pins are interchangable, but please reference the arduino
 *  documentaiton to be sure. 
 *  
 *  Author:         Jake Ketchum
 *  Last Changed:   7/14/2017
 */
 
// Z axis stepper pins. 
#define PIN_Z_ENABLE     0
#define PIN_Z_DIRECTION  4
#define PIN_Z_PULSE      5
#define PIN_Z_ENDSTOP    9

 
// X axis stepper pins. 
#define PIN_X_ENABLE     0
#define PIN_X_DIRECTION  3
#define PIN_X_PULSE      2
#define PIN_X_ENDSTOP    10


// C axis stepper pins. 
#define PIN_C_ENABLE     0
#define PIN_C_DIRECTION  7
#define PIN_C_PULSE      6
#define PIN_C_ENDSTOP    8

// Spindle Motor Pins.
#define PIN_S 11

// axis indecicis. 
#define XIN 0
#define ZIN 1
#define CIN 2

// G30 location. 
#define G30X 2000   //thou
#define G30Z 100   //thou
#define G30C 110   //degrees

// defaults
#define DEFAULT_MOVE_FEED 10

#define Converstion_Speed_Z  0.5// s/mm to milliseconds per step. 
#define Converstion_Speed_X  1// s/mm to milliseconds per step.
#define Converstion_Speed_C  1// s/mm to milliseconds per step.

#define Conversion_Displacment_X 20.6896551724 // Steps per mm
#define Conversion_Displacment_Z 20.6896551724 // Steps per mm
#define Conversion_Displacment_C 59.2          // Steps per Degree

#define REVERSE_Z false
#define REVERSE_X false
#define REVERSE_C true

// General
#define TIMEOUT 50 // Keep this low to avoid jumps when new commands are recieved.

// Homing Config. 
#define HOME_SPEED 1 
#define HOME_STEP 10   
#define HOME_SPEED_C 10

 // G-Code commands.
String COMMAND_STOP   = "M00"; // no arguments.
String COMMAND_RAPID  = "G0";  // X, Z, C
String COMMAND_PAUSE = "M123";
String COMMAND_RESUME = "M124";
