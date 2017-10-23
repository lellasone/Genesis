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
 #define PIN_Z_DIRECTION  2
 #define PIN_Z_PULSE      3
 #define PIN_Z_ENDSTOP    0

 
// X axis stepper pins. 
 #define PIN_X_ENABLE     0
 #define PIN_X_DIRECTION  4
 #define PIN_X_PULSE      5
 #define PIN_X_ENDSTOP    0


// C axis stepper pins. 
 #define PIN_C_ENABLE     0
 #define PIN_C_DIRECTION  6
 #define PIN_C_PULSE      7
 #define PIN_C_ENDSTOP    0

// axis indecicis. 
 #define XIN 0
 #define ZIN 1
 #define CIN 2

// G30 location, given in steps. 
#define G30X 100
#define G30Z 100
#define G30C 0 // given in degrees

// defaults
#define DEFAULT_MOVE_FEED 10

 // 
 #define Converstion_Speed_Z  1// mm/s to milliseconds per step. 
 #define Converstion_Speed_X  1// mm/s to milliseconds per step.
 #define Converstion_Speed_C  1// mm/s to milliseconds per step.
 #define Conversion_Displacment_X 10 // TODO:Calculate.
 #define Conversion_Displacment_Z 10 // TODO: calculate.
 // General
 #define TIMEOUT 50 // Keep this low to avoid jumps when new commands are recieved.

 // G-Code commands.
String COMMAND_STOP   = "M00"; // no arguments.
String COMMAND_RAPID  = "G0";  // X, Z, C
