# Genesis
Control Code for CNC Profile Cutter


Implimented GCode commands: 
    G1 - "G1, Xn, Zn, An, Fn"   Controlled move, sets new cordiantess (absolute or reletive depending setting). Note, that the feedrate is applied
                                to each axis independently of the others. 
    M0 - "M0"                   Emerency stop, clears queue and kills motors. 
    M123 - "M123"               Pause execution, commands can still be added to the queue, but they will not be executed. Used to start program
                                transmisions. 
    M124 - "M124"               Resume execution, tells the device to resume executing commands. Used to end program transmissions. 



Keyway File Format: 
    Keyway data is stored in .csv files, which are read into the GUI on startup, and used to translate codes into cut paramiters. 
    The exact format of each .csv file would be best explored by looking at some of the example files. However, it is important to 
    note that all listed paramiters and values must have a listed value. Further details on indevidual paramiters can be found below. 
    
    Paramiters: 
        _num_pins           Int, the number of wells. 
        _name               String, Displayed in drop down. 
        _workpiece_height   int, height of the blank in thou. 
        _safe_z             int, safe movment height for cutter, must be larger than workpiece height in thou.
        _feed_rate_cut      int, cutter speed per axis during metal contact moves in thou per second. 
        _default_code       String, default code when this keyway selected. 
        _segement_length    Int, number of char per well in codes for this keyway. 
    
    Spacing: 
        
        
    well codes:
        well codes are stored as a dictionary of 