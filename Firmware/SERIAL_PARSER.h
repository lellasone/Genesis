//#include "MOTORS.h"
#include <QueueList.h>

QueueList<String> CommandQueue;

void commandStop(){
  /* Emergency stop command. Executed imediatly upon recipt and
   *  takes no arguments. Stops all motors and clears the command
   *  queue. 
   */
   
  // stop all movement.
  TargetX = CurrentX;
  TargetZ = CurrentZ;
  TargetC = CurrentC;
  
  //clear command queue
  while (!CommandQueue.isEmpty()){
    String temp = CommandQueue.pop();
  }
}

void M123(){
  /*
   *    M123 command, pauses execution of the command queue. This is principally 
   *    used to begin recipt of a new GCode program. 
   */
  Serial.println("pausing Queue");
  runQueue = false;
}

void M124(){
  /*
   * M124 command, resumes execution of the command queue. This is principally used to 
   * begin execution of a GCode program after recipt of a GCode program. 
   */
  runQueue = true; 
}




void serialEvent(){
  /*
   * Responds to an incomming G-Code command. Reacts if nessary, otherwise checks that the 
   * command string corresponds to a valid command and then copies the entire string
   * including arguments to the queue. Commands are parsed when each new command is 
   * poped for execution. 
   */
  String command_new = Serial.readString();
  if (command_new == COMMAND_STOP){
    commandStop();
  } else if (command_new == COMMAND_PAUSE){
    M123();
  } else if (command_new == COMMAND_RESUME){
    M124();
  }
  // TODO: Add a check that this is a valid command.  
  CommandQueue.push(command_new);
}

