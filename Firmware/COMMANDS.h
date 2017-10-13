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
   *  The command is in the format:
   *    G1 Xn Zn Cn Fn 
   *  where n is a numeric value. 
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
  Serial.println("Setting New G1 Cords");
  Serial.println(String(New_C));
  // Set new target locations. 
  Target[ZIN] = int(New_Z * Conversion_Displacment_Z);
  Target[XIN] = int(New_X * Conversion_Displacment_X);
  Target[CIN] = int(New_C);

  // Set new speeds. 
  Time[ZIN] = New_F * Converstion_Speed_Z;
  Time[XIN] = New_F * Converstion_Speed_X;
  Time[CIN] = New_F * Converstion_Speed_C;

  // clear timers. Needed in case there was a delay between
  // this move call and the last. 
  Timer[ZIN] = 0;
  Timer[XIN] = 0;
  Timer[CIN] = 0;
}

bool G1::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed.
   * 
   * Returns: True if incomplete, False if complete. 
   */
  if (Current[XIN] != Target[XIN]){
    return true;
  }
  if (Current[ZIN] != Target[ZIN]){
    return true;
  }
  if (Current[CIN] != Target[CIN]){
    return true;
  }
  Serial.println("G1 Complete");
  return false;
}


