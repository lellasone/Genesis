# Genesis
Control Code for CNC Profile Cutter

GCode:
   Genesis impliments a very stripped down version of standard GCode. Commands which are guaranteed to work are listed below. Unless otherwise noted "M" commands will be executed imediatly upon recipt while "G" commands are stored in a queue and executed                sequentially by the mill. All commands should be sent in the format "Gn, (args)" where the args are comma delinated,
   but not order sensative. 

    Implemented GCode commands: 
        G1 - "G1, Xn, Zn, An, Fn"   Controlled move, sets new coordinates (absolute or relative depending setting). Note, that the feedrate is applied
                                    to each axis independently of the others. 
        G90 - "G90"                 Sets the target device to absolute distance mode. 
        G91 - "G91"                 Sets the target device to relative distance mode. 
        M0 - "M0"                   Emergency stop, clears queue and kills motors. 
        M123 - "M123"               Pause execution, commands can still be added to the queue, but they will not be executed. Used to start program
                                    transmissions. 
        M124 - "M124"               Resume execution, tells the device to resume executing commands. Used to end program transmissions. 



Keyway File Format: 
    Keyway data is stored in .csv files, which are read into the GUI on startup, and used to translate codes into cut paramiters. 
    The exact format of each .csv file would be best explored by looking at some of the example files. However, it is important to 
    note that all listed parameters and values must have a listed value. Further details on individual parameters can be found below. 
    
    Parameters: 
        _num_pins           Int, the number of wells. 
        _name               String, Displayed in drop down. 
        _workpiece_height   int, height of the blank in thou. 
        _safe_z             int, safe movement height for cutter, must be larger than workpiece height in thou.
        _feed_rate_cut      int, cutter speed per axis during metal contact moves in thou per second. 
        _default_code       String, default code when this keyway selected. 
        _segement_length    Int, number of char per well in codes for this keyway. 
    
    Spacing: 
        #TODO: finish this section. 
        Spacing is stored as a dictionary in the form {well_num: spacing_value}.
        Spacings are stored in the appropriate .csv file under the heading "Default Spacing". Spacings are in 
        units of thou.
        
    well codes:
        #TODO: finish this section. 
