/*
 * This file contains the command objects used to control genesis. 
 * Each one should correspond to a GCode command. When a GCode
 * command is recognized it's corresponding object (and arguments)
 * will be added to the main command queue. Commands are popped off
 * one at a time and executed. 
 * 
 * Each command object must have: 
 *  - One public function "execute" which carries out the settings changes. 
 *  - One public function "hold" which instructs the main loop to wait while
 *    the command is being carried out. For commands which do not need a wait,
 *    this can simply return "false" otherwise it should return "true" while
 *    a wait state is required. 
 *    
 * The following GCode commands have been implimented: 
 *    
 * Author:        Jake Ketchum
 * Last Changed:  7/15/17
 */
class GCommand{
  /*
   * parrent class for all G-Code commands. When a new G-Code command is 
   * poped it will be stored as a GCommand object.  As such, all G-Code 
   * commands should be created as children of GCommand
   * 
   * All G-Code commands must have: 
   * Constructor - Takes any appropriate arguments. 
   * execute     - Called once per command, takes any actions
   *               associated with that command. (Changing 
   *               target variables, reading data, ect.
   * hold        - Responcible for waiting until the unit has
   *               fully responded to the command. Should return
   *               True while wait is required, and False
   *               thereafter. If no wait is required this can
   *               simply return False.
   */
  public:
    virtual void execute() = 0;
    virtual bool hold() = 0;
};



class G1 : public GCommand{
  /* 
   *  Controlled move command. Requires that all 4 arguments be 
   *  given. Negative values can be used to indicate no change. 
   *  This move type should be used for all linear cutting. 
   *  
   *  Note: Feedrate will be applied as a per-axis speed cap 
   *  rather than a cap on the total movment speed. 
   */
  long New_X, New_Z, New_C, New_F;
  public:
    G1(long,long,long,long);
    void execute();
    bool hold();
 };


  
G1::G1(long X, long Z, long C, long F){
  New_X = X;
  New_Z = Z;
  New_C = C;
  New_F = F;
}

void G1::execute (){
  // Set new target locations. 
  TargetZ = int(New_Z * Conversion_Displacment_Z);
  TargetX = int(New_X * Conversion_Displacment_X);
  TargetC = int(New_C);

  // Set new speeds. 
  TimeZ = New_F * Converstion_Speed_Z;
  TimeX = New_F * Converstion_Speed_X;
  TimeC = TargetC * TIME_PER_DEGREE;

  // clear timers. Needed in case there was a delay between
  // this move call and the last. 
  TimerZ = 0;
  TimerX = 0;
  TimerC = 0;
}

bool G1::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed.
   * 
   * Returns: True if incomplete, False if complete. 
   */
  if (CurrentX != TargetX){
    return true;
  }
  if (CurrentZ != TargetZ){
    return true;
  }
  if (TimerC >= TimeC){
    return true;
  }
  return false;
}


