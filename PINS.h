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
 #define PIN_Z_ENABLE     19
 #define PIN_Z_DIRECTION  18
 #define PIN_Z_PULSE      17
 #define PIN_Z_ENDSTOP    16

 // 
 #define TIME_PER_DEGREE      100// time for the servo to move one degree in milliseconds. 
 #define Converstion_Speed_Z  10// mm/s to milliseconds per step. 
 #define Converstion_Speed_X  10// mm/s to milliseconds per step.
 #define Conversion_Displacment_X 10 // TODO:Calculate.
 #define Conversion_Displacment_Z 10 // TODO: calculate.
 // General
 #define TIMEOUT 50 // Keep this low to avoid jumps when new commands are recieved.

 // G-Code commands.
String COMMAND_STOP   = "M00"; // no arguments.
String COMMAND_RAPID  = "G0";  // X, Z, C
