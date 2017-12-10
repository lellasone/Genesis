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
   *    "G1,Xn,Zn,Cn,Fn"
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
  if (absolute){
    Target[ZIN] = int(New_Z * Conversion_Displacment_Z);
    Target[XIN] = int(New_X * Conversion_Displacment_X);
    Target[CIN] = int(New_C * Conversion_Displacment_C);
  } else {
    Target[ZIN] += int(New_Z * Conversion_Displacment_Z);
    Target[XIN] += int(New_X * Conversion_Displacment_X);
    Target[CIN] += int(New_C * Conversion_Displacment_C);
  }

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



class G30 : public GCommand{
  /* 
   *  Fast move command to a position 1 inch directly above the bottom of the vice,
   *  at the the sholder-stop of the workpiece with angle = 0. This is used to establish
   *  a consistent location reletive to the work-piece for reletive move commands. 
   *  
   *  The command is in the format:
   *    G30
   */
  long New_X, New_Z, New_C, New_F;
  public:
    G30();
    void execute();
    bool hold();
 };
  
G30::G30(){
  New_X = G30X * Conversion_Displacment_X;
  New_Z = G30Z * Conversion_Displacment_Z;
  New_C = G30C * Conversion_Displacment_C;
  New_F = DEFAULT_MOVE_FEED;
}

void G30::execute (){
  // Set new target locations. 
  Target[ZIN] = int(New_Z);
  Target[XIN] = int(New_X);
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

bool G30::hold (){
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

class G90 : public GCommand{
  /* 
   *  Switches from relative to absolute positioning mode. When this command
   *  is sent, subsequent G1 and G0 commands will be interpreted as cords
   *  reletive to the device's zero location. 
   *  
   *  The command is in the format:
   *    G90
   */
  public:
    G90();
    void execute();
    bool hold();
 };
  
G90::G90(){
  
}

void G90::execute (){
  absolute = true; 
}

bool G90::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed. This will alwasy be false.
   * 
   * Returns: True if incomplete, False if complete. 
   */
  return false;
}


class G91 : public GCommand{
  /* 
   *  Switches from absolute to relative positioning mode. When this command
   *  is sent, subsequent G1 and G0 commands will be interpreted as cords
   *  reletive to the device's location when that command is executed. 
   *  
   *  The command is in the format:
   *    G91
   */
  public:
    G91();
    void execute();
    bool hold();
 };
  
G91::G91(){
  
}

void G91::execute (){
  absolute = false; 
}

bool G91::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed. This will alwasy be false.
   * 
   * Returns: True if incomplete, False if complete. 
   */
  return false;
}


class G28 : public GCommand{
  /* 
   *  
   *  
   *  The command is in the format:
   *    G28
   */
  bool touch_x;
  bool touch_z;
  bool touch_c;
  bool release_x;
  bool release_z;
  bool release_c;
  private:
    bool touchAndRelease(int, int, double);
    bool touch(int, int, double);
  public:
    G28();
    void execute();
    bool hold();
 };
  
G28::G28(){
  touch_x = false;
  touch_z = false;
  touch_c = false;
  release_x = false;
  release_z = false;
  release_c = false;
}

void G28::execute (){
  Time[ZIN] = HOME_SPEED * Converstion_Speed_Z;
  Time[XIN] = HOME_SPEED * Converstion_Speed_X;
  Time[CIN] = HOME_SPEED_C * Converstion_Speed_C;
  Target[XIN] = Current[XIN];
  Target[CIN] = Current[CIN];
  Target[ZIN] = Current[ZIN];
}

bool G28::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed.
   * 
   * Returns: True if incomplete, False if complete. 
   */
   // move each axis until it touches it's limit switch.
  
  Serial.print(digitalRead(PIN_X_ENDSTOP));
  Serial.print(",");
  Serial.print(digitalRead(PIN_Z_ENDSTOP));
  Serial.print(",");
  Serial.print(digitalRead(PIN_C_ENDSTOP));
  Serial.print(",,");
  Serial.print(touch_x);
  Serial.print(",");
  Serial.print(touch_z);
  Serial.print(",");
  Serial.print(touch_c);
  Serial.print(",,");
  Serial.print(release_x);
  Serial.print(",");
  Serial.print(release_z);
  Serial.print(",");
  Serial.println(release_c);

  if (!touch_x){
    if (digitalRead(PIN_X_ENDSTOP)){
      Current[XIN] = Target[XIN];
      touch_x = true;
    } else {
      if (Current[XIN] <= Target[XIN]){
        Serial.println("Test");
        Target[XIN] -= int(HOME_STEP * Conversion_Displacment_X);
      }
    }
  }
  
  if (!touch_z){
   touch_z = touch(ZIN, PIN_Z_ENDSTOP, Conversion_Displacment_Z);
  }
  
  if (!touch_c){
    touch_c = touch(CIN, PIN_C_ENDSTOP, Conversion_Displacment_C);
  }
  
  // move each axis until the respective limit switch has been released. 
  
  if (touch_x && !release_x){
    release_x = touchAndRelease(XIN, PIN_X_ENDSTOP, Conversion_Displacment_X);
  }
  
  if (touch_z && !release_z){
    release_z = touchAndRelease(ZIN, PIN_Z_ENDSTOP, Conversion_Displacment_Z);
  }

  if (touch_c && !release_c){
    release_c = touchAndRelease(CIN, PIN_C_ENDSTOP, Conversion_Displacment_C);
  }
    
  if (release_c && release_x && release_z){
    Serial.println("Homing Complete");
    return false;
  } else {
    return true;
  }
}

bool G28::touchAndRelease(int index, int endstopPin, double displacment){
    if (!digitalRead(endstopPin)){
      Current[index] = 0;
      Target[index] = Current[index];
      return(true);
    } else {
      if (Current[index] >= Target[index]){
        Target[index] += int(HOME_STEP * displacment);
      }
      return(false);
    }
  }

bool G28::touch(int index, int endStopPin, double displacment){
  if (digitalRead(endStopPin)){
    Target[index] = Current[index];
    return true;
  } else {
    if (Current[index] <= Target[index]){
      Target[index] -= int(HOME_STEP * displacment);
    }
    return false;
  }
}

class G99 : public GCommand{
  /* 
   *  Sets the speed of the spindle as a value from 0-100. Values outside of that range
   *  will be ignored and an emergency stop command will be called. 
   *  
   *  The command is in the format:
   *    G99 Sn Where n is a value between 0 - 100
   */
  int newSpeed; 
  public:
    G99(int);
    void execute();
    bool hold();
 };
  
G99::G99(int Speed){
  Serial.println(Speed);
  if( Speed > 100 || Speed < 0){
    Serial.print("Spindle Speed is Invalid: ");
    Serial.println(Speed);
    commandStop;
  }
  newSpeed = Speed;
}

void G99::execute (){
  SpeedS = newSpeed; 
}

bool G99::hold (){
  /*
   * This function is called repeatedly to check whether the command has 
   * been completed. This will alwasy be false.
   * 
   * Returns: True if incomplete, False if complete. 
   */
  return false;
}
