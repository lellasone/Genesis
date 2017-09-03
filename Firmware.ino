/*
 * 
 * Axis: 
 *    X - Travel along the workpiece. 
 *    Z - Travel into the work piece (towords the cutter). 
 *    C - Work piece angle about Z.
 *    S - Not an axis, references cutter speed. 
 *    
 * Commands:
 *    M00 - Emergency stop.   Arguments: None.
 *    G0  - Rapid move.       Arguments: X,Z,C
 */

#include "MOTORS.h"
//#include "PINS.h"
#include "SERIAL_PARSER.h"
#include "COMMANDS.h"

char *tokenPointer;

void setup() {
  // put your setup code here, to run once:
  setupX();
  Serial.begin(115200);
  Serial.setTimeout(TIMEOUT);
  Serial.println("Serial Ready");
}

void setupX(){
  // enable stepper motor driver. 
  pinMode(PIN_Z_ENABLE, OUTPUT);
  digitalWrite(PIN_Z_ENABLE, HIGH);
  pinMode(PIN_Z_DIRECTION, OUTPUT);
  pinMode(PIN_Z_PULSE, OUTPUT);
}


void loop() {
  if (!CommandQueue.isEmpty()){
    GCommand *command = getNewCommand();
    Serial.println("");
  } else {
  }
  //TODO: Make this an interrupt.
  updateMotors();
}

GCommand * getNewCommand(){
  /* #TODO: finish comment. 
   *  Get the command string (Command + arguments)
   * Turn the string object into an array of chars. 
   * Tokenize the char array to get the command itself. 
   * 
   */
  // get string as chars.
  String commandString = CommandQueue.pop();
  int len = commandString.length();
  char buf[len + 1];
  buf[len + 1] = 0;
  tokenPointer = buf;
  commandString.toCharArray(buf, len + 1);
  //Serial.println(buf);
  
  // parse command string for tokens. 
  String command;
  String args[6];
  command = tokenize(',');
  args[0] = tokenize(',');
  args[1] = tokenize(',');
  args[2] = tokenize(',');
  args[3] = tokenize(',');

  if( args[0][0] = 'G'){
    process_G(args);
  } else {
    Serial.print("Command Error, Invalid first letter: " + args[0] );
  }
}

void process_G(String command[]){
  /* This function is responcible for processing
   *  G-Codes starting with G. It determines the 
   *  correct G-Code and then adds that to the queue. 
   *  
   *  Args: 
   *    command - an array of Strings containing the command and each 
   *              argument. 
   */
   Serial.print("processing G command: ");
}

String tokenize (char sep){
  /* This function parses the current command string for the 
   * sep char and then returns the token preceding it. It requires
   * the existence of a 128 byte buffer pointed to by the global 
   * variable "tokenPointer". "tokenPointer" is modified to point
   * to the char after the sep char each time this function is called. 
   * 
   * Args:
   *  sep - seperation value, single char. 
   *  
   * Returns: chars in buf preceding the first instance of the sep char. 
   */
   int i = 0;
   char token[128];
   while (true){
    char current = tokenPointer[i];
    // check loop end conditions.
    if (current == sep){break;}
    if (current == NULL){break;}

    token[i] = tokenPointer[i];
    i++;
   }
   token[i] = 0;
   tokenPointer += i + 1;
   return String(token);
}

