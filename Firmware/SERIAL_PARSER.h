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
  }
  // TODO: Add a check that this is a valid command.  
  CommandQueue.push(command_new);
}

