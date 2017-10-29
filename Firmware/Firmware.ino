/*    Genesis Firmware
 *     
 * 
 * Axis: 
 *    X - Travel along the workpiece. 
 *    Z - Travel into the work piece (towords the cutter). 
 *    C - Work piece angle about Z.
 *    S - Not an axis, references cutter speed. 
 *    
 * Commands:
 *    Possible commands are lised below. All commands are case sensative and
 *    should be sent with comma delinated arguments in the order specified. 
 *    Unless otherwise noted, arguments are order sensetive. 
 *    M00 - Emergency stop.   Arguments: None.
 *    G1  - Rapid move.       Arguments: X,Z,C,F
 */

char *tokenPointer;
bool runQueue; // if false queue execution pause. 
bool absolute; // if true move commands in absolute cords.

#include "MOTORS.h"
//#include "PINS.h"
#include "SERIAL_PARSER.h"
#include "COMMANDS.h"


void setup() {
  // put your setup code here, to run once:
  setupX();
  setupZ();
  setupC();
  Serial.begin(115200);
  Serial.setTimeout(TIMEOUT);
  Serial.println("Serial Ready");
  runQueue = true;
}

void setupX(){
  // enable stepper motor driver. 
  pinMode(PIN_X_ENABLE, OUTPUT);
  digitalWrite(PIN_Z_ENABLE, HIGH);
  pinMode(PIN_X_DIRECTION, OUTPUT);
  pinMode(PIN_X_PULSE, OUTPUT);
}
void setupZ(){
  // enable stepper motor driver. 
  pinMode(PIN_Z_ENABLE, OUTPUT);
  digitalWrite(PIN_Z_ENABLE, HIGH);
  pinMode(PIN_Z_DIRECTION, OUTPUT);
  pinMode(PIN_Z_PULSE, OUTPUT);
}

void setupC(){
  // enable stepper motor driver. 
  pinMode(PIN_C_ENABLE, OUTPUT);
  digitalWrite(PIN_C_ENABLE, HIGH);
  pinMode(PIN_C_DIRECTION, OUTPUT);
  pinMode(PIN_C_PULSE, OUTPUT);
}



void loop() {
  if (!CommandQueue.isEmpty() && runQueue){
    GCommand *command = getNewCommand();
    command->execute();
    while (command->hold()){
      //TODO: Make this an interrupt.
      updateMotors();
    }
  } else {
    delay(1);
  }
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
  args[0] = tokenize(',');
  args[1] = tokenize(',');
  args[2] = tokenize(',');
  args[3] = tokenize(',');
  args[4] = tokenize(',');

  if( args[0][0] = 'G'){
    return(process_G(args));
  } else {
    Serial.print("Command Error, Invalid first letter: " + args[0] );
  }
}

GCommand * process_G(String command[]){
  /*  This function is responcible for processing
   *  G-Codes starting with G. It takes a command  
   *  string and generates the corresponding GCommand
   *  object. Which is then returned. 
   *  
   *  Args: 
   *    command - an array of Strings containing the command and each 
   *              argument. 
   *  Returns: 
   *    coresponding GCommand object. 
   */
   Serial.println("processing G command: " + String(command[0]));
   GCommand * newCommand;
   switch (command[0][1]) {
    case '1':{
      Serial.println("Processing G1");
      long X = getArg(command, 'X', 5);
      long Z = getArg(command, 'Z', 5);
      long C = getArg(command, 'C', 5);
      long F = getArg(command, 'F', 5);
      newCommand = new G1(X,Z,C,F);
      break;
    }
    case '3':{
      Serial.println("Processing G30");
      newCommand = new G30();
    }
  }
   return(newCommand);
}

long getArg(String command[], char arg, int len){
/*
 * This function takes a command string, and returns the numeric value 
 * corresponding to the specified argument char. This is used to ensure 
 * commands are not order sensetive. 
 * 
 * Args:
 *  command - A command array. Entries should be in the form "[letter][number]"
 *  arg - the letter coresponding to the argument desired. For example "X"
 *  len - the length of the command array. 
 */

 for(int i = 0; i < len; i++){
   Serial.println(command[i][1]);
  if(command[i][0] == arg){
    return command[i].substring(1).toInt();
  }
 }
 // something has gone wrong. 
 Serial.println("Argument" + String(arg) + "is missing");
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

