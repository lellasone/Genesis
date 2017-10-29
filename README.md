# Genesis
Control Code for CNC Profile Cutter


Implimented GCode commands: 
G1 - "G1, Xn, Zn, An, Fn"   Controlled move, sets new cordiantess (absolute or reletive depending setting). Note, that the feedrate is applied
                            to each axis independently of the others. 
Mo - "M0"                   Emerency stop. CLears queue and kills motors. 
M123 - "M123"               Pause execution, commands can still be added to the queue, but they will not be executed. Used to start program
                            transmisions. 
M124 - "M124"               Resume execution, tells the device to resume executing commands. Used to end program transmissions. 
