import Tkinter as tk
import tkFileDialog
import os

class generic():
    _data_folder = "data"
    _keyway_dict = {}
    _default_type = "Sargent - DG1"

    # Keyway data
    _num_pins = 7
    _feed_rate_move = 1 #thou/second non-contact moves.
    _feed_rate_cut = 2    #thou/second contact moves.
    _safe_Z = 1000 #Given in thou, reletive to the base of the workpiece. Should
    _workpiece_height = 250 # Z height of work piece reletive to vice.

    _well_spacings = []
    _well_angles = []
    _well_depths = []

    # arrays of IntVars used to back GUI
    _backing_well_spacings = []
    _backing_well_angles = []
    _backing_well_depths = []


    #Constants
    _DEFAULT_DEPTH = 0
    _DEFAULT_SPACING = 300
    _DEFAULT_ANGLE = 0
    _G30 = {1000, 0, 0} #G30 reletive to bottom of workpiece sholder stop

    def __init__(self):
        self._GUI_mode = False
        self._import_workpiece_types(self._data_folder)
        self.current_keyway = self._keyway_dict[self._default_type]
        self._backing_type = tk.StringVar()  #Stores the current model metal stock.
        self._backing_type.set(self._default_type)

        self._backing_height = tk.IntVar()
        self._backing_num_pins = tk.IntVar()
        self._backing_num_pins.set(self._num_pins)
        self._backing_code = tk.StringVar()
        self._backing_code.set(" ")
        self._backing_safe_z = tk.IntVar()
        self._backing_safe_z.set(self._safe_Z)

        # backing variables for offsets.
        self._backing_depth_offset = tk.IntVar()
        self._backing_depth_offset.set(0)
        self._backing_spacing_offset = tk.IntVar()
        self._backing_spacing_offset.set(0)
        self._backing_angle_offset = tk.IntVar()
        self._backing_angle_offset.set(0)

        self._backing_height.set(0)
        self._set_new_type(self.current_keyway)
        pass

    def _import_workpiece_types(self, data):
        file_names = os.listdir(data)
        index_paramiter = 0
        index_paramiter_value = 1
        index_well_num = 2
        index_default_spacing = 3
        index_well_codes = 4
        index_height = 5
        index_angle = 6
        index_spacing_offset = 7

        for file_name in file_names:
            local_keyway = keyway()
            try:
                file = open(data + "/" + file_name, 'r')
                line = file.readline()
                print line
                line = file.readline()
                while line:
                    line = line.split(",")
                    print line
                    local_keyway.paramiters.update({line[index_paramiter]:line[index_paramiter_value]})
                    local_keyway.spacings.update({line[index_well_num]:line[index_default_spacing]})
                    local_keyway.well_codes.update({line[index_well_codes]:[line[index_height],
                                                                                line[index_angle],
                                                                                line[index_spacing_offset]]})
                    line = file.readline()

                self._keyway_dict.update({local_keyway.paramiters["_name"]: local_keyway})
            except Exception as e:
                print "file exception: " + str(e)
                self._handle_file_exception()

    def _handle_file_exception(self,e):
        pass

    def return_gui_frame(self, master, mode):
        self.subframe = tk.Frame(master)
        basic = self.basic_frame(self.subframe)
        self.advanced_frame = self.generate_advanced_frame(self.subframe)

        basic.grid(row = 0, column = 0)
        if mode: self.advanced_frame.grid(row = 0, column = 1)
        self._GUI_mode = True
        return self.subframe

    def basic_frame(self, master):
        subframe = tk.Frame(master)
        stock_label = tk.Label(subframe, text = "Type: ")
        print list(self._keyway_dict.keys())
        stock = tk.OptionMenu(subframe,
                              self._backing_type,
                              *list(self._keyway_dict.keys()),
                              command = lambda x: self._set_new_type(self._keyway_dict[self._backing_type.get()]))
        stock.config(width = 19)
        code = self._return_label_entry(subframe,
                                        "Code: ",
                                        self._backing_code,
                                        width = 25)
        save = tk.Button(subframe, text = "Save",
                         width = 27,
                         command = self._save_code)
        run = tk.Button(subframe,
                        text = "Run",
                        width = 27)


        stock_label.grid(row = 0, column = 0)
        stock.grid(row = 0, column = 1, columnspan = 2)
        code.grid(row = 1, column = 0, columnspan = 2)
        save.grid(row = 2, column = 0, columnspan = 2)
        run.grid(row = 3, column = 0, columnspan = 2)
        return subframe

    def generate_advanced_frame(self, master):
        subframe = tk.Frame(master)
        title = tk.Label(subframe, text = "Workpiece Type: 'Generic, {0} pin'".format(self._num_pins))
        pins = self._return_pins_frame(subframe)
        params = self._paramiters_frame(subframe)
        offsets = self.offset_frame(subframe)

        title.grid(row = 0, column = 0, columnspan = 2)
        pins.grid(row = 1, column = 1, columnspan = 2)
        offsets.grid(row = 1, column = 3, columnspan = 1)
        params.grid(row = 1, column = 4, sticky = 'W')

        return subframe

    def _paramiters_frame(self, master):
        subframe = tk.Frame(master, padx = 20)
        title = tk.Label(subframe, text = "General Parameters")
        height = self._return_label_entry(subframe,
                                          "Stock Height",
                                          self._backing_height)
        num_pins = self._return_label_entry(subframe,
                                            "# Pins",
                                            self._backing_num_pins)
        safe_z = self._return_label_entry(subframe,
                                          "Safe Height",
                                          self._backing_safe_z)


        title.grid(row = 0, column = 0, sticky = 'N')
        height.grid(row = 1, column = 0, sticky = 'E')
        num_pins.grid(row = 2, column = 0, sticky = 'E')
        safe_z.grid(row = 3, column = 0, sticky = 'E')

        return subframe

    def _return_label_entry(self, master, text, textVar , callback = "null", width = 15):
        """
        Conveniece function for generating a single frame with a label and
        entry.
        :param master: TK instance.
        :param text: The text for the label.
        :param textVar: The backing variable for the Entry.
        :return:
        """
        subframe = tk.Frame(master)
        label = tk.Label(subframe, text = text)
        entry = tk.Entry(subframe, textvariable = textVar, width = width)
        if callback != "null": entry.bind("<Return>", callback)
        label.grid(column = 0, row = 0)
        entry.grid(column = 1, row = 0)
        return subframe

    def _return_pins_frame(self, master):
        """
        This creates the Pins sub-frame. It holds the Entry objects for the angle
        depth, and spacing (distancce from previous) for each pin as well as
        associated labels. This method should be over-written by custom workpiece
        script classes.

        :param master: Frame for the script GUI.
        :return: frame containing depth, angle, spacing entry and label objects.
        """
        subframe = tk.Frame(master)

        depth_label = tk.Label(subframe, text = "Depth")
        spacing_label = tk.Label(subframe, text = "Spacing")
        angle_label = tk.Label(subframe, text = "Angle")
        depth_label.grid(row = 1, column = 0)
        spacing_label.grid(row = 2, column = 0)
        angle_label.grid(row = 3, column = 0)

        for i in range(0, self._num_pins):
            name = tk.Label(subframe, text = "Pin " + str((i + 1)))
            depth = tk.Entry(subframe, textvariable = self._backing_well_depths[i])
            angle = tk.Entry(subframe, textvariable = self._backing_well_angles[i])
            spacing = tk.Entry(subframe, textvariable = self._backing_well_spacings[i])

            name.grid(row = 0, column = i + 1)
            depth.grid(row = 1, column = i + 1)
            angle.grid(row = 3, column = i + 1)
            spacing.grid(row = 2, column = i + 1)

        return subframe

    def offset_frame(self, master):
        """
        This creates a frame containing an entry for each of the:
        global angle offset, global depth offset, global spacing offset.
        This frame should only be shown when the GUI is in advanced mode.
        :param master: Tkinter frame for advanced mode.
        :return: subframe containing offset entries.
        """
        subframe = tk.Frame(master)
        title = tk.Label(subframe,
                         text = "Offsets")
        depth = tk.Entry(subframe,
                         textvariable = self._backing_depth_offset)
        spacing = tk.Entry(subframe,
                           textvariable = self._backing_spacing_offset)
        angle = tk.Entry(subframe,
                         textvariable = self._backing_angle_offset)

        title.grid(row = 0, column = 0, pady = 0)
        depth.grid(row = 1, column = 0, pady = 1)
        spacing.grid(row = 2, column = 0, pady = 1)
        angle.grid(row = 3, column = 0, pady = 1)
        return subframe

    def set_lists_default(self):
        self._backing_well_angles = []
        self._backing_well_depths = []
        self._backing_well_spacings = []

        self._backing_well_angles = self.generate_list(self._backing_well_angles,
                                                       self._num_pins,
                                                       self._DEFAULT_ANGLE)

        self._backing_well_depths = self.generate_list(self._backing_well_depths,
                                                       self._num_pins,
                                                       self._DEFAULT_DEPTH)
        # set default spacings for this keyway

        for i in sorted(self.current_keyway.spacings.keys()):
            if i.isdigit():
                print i
                real_spacing = self.current_keyway.spacings[i]
                temp = tk.IntVar()
                temp.set(real_spacing)
                self._backing_well_spacings.append(temp)

    def generate_list(self, list, length, value):
        for i in range(0, length):
            temp = tk.IntVar()
            temp.set(value)
            list.append(temp)
        return list

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Callbacks:

    def _save_code(self):
        self._take_snapshot()
        code = self.generate(self._well_depths, self._well_angles, self._well_spacings)
        name = tkFileDialog.asksaveasfilename(defaultextension='.txt')

        self._print_array(code)
        self._write_code_file(name, code)

    def _write_code_file(self, name, code):
        try:
            file = open(name, 'w')
            for line in code:
                file.write(line)
                file.write(os.linesep)
            file.close()
        except IOError as e:
            print "IOError has occured during save: [{0}]".format(e)

    def _take_snapshot(self):
        self._num_pins = self._backing_num_pins.get()
        self._workpiece_height = self._backing_height.get()

        self._well_spacings = self._convert_tk_array(self._backing_well_spacings)
        self._well_depths = self._convert_tk_array(self._backing_well_depths)
        self._well_angles = self._convert_tk_array(self._backing_well_angles)

    def _print_array(self, array):
        for string in array:
            print string

    def _convert_tk_array(self, tk_array):
        """
        Takes an array of TK vars, and generates an array containing there
        corresponding values.
        :param tk_array:
        :param array:
        :return:
        """
        array = []
        for i in range(0, self._num_pins):
            array.append(tk_array[i].get())
        return array

    def _set_new_type(self, type):
        self.current_keyway = type
        self._backing_num_pins.set(type.paramiters["_num_pins"])
        self._num_pins = int(type.paramiters["_num_pins"])
        self._backing_safe_z.set(type.paramiters["_safe_Z"])
        self._feed_rate_cut = type.paramiters["_feed_rate_cut"]
        self.set_lists_default()
        if self._GUI_mode:
            self.advanced_frame.destroy()
            self.advanced_frame = self.generate_advanced_frame(self.subframe)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Script:
    def generate(self, well_heights, well_angles, well_spacings):
        """
        This function is responcible for generating a complete G-Code program
        for the specified angles, heights, and spacings. The resultant spacing
        program includes the start and end code specified in those functions,
        and is intended to be run as a block.
        :param well_heights: an int array giving the height of each well as
                             measured from the base of the workpiece to the
                             bottom of the desired cut.
        :param well_angles:   an int array specifying the angle of each cut in
                             degrees. At zero the cutter is perpendicular to
                             the work piece. Positive angles will result in
                             counterclockwise rotation.
        :param well_spacings: An int array specifying the gap between the
                             previous cutter location and the new well.
                             Spacings are assumed to be given as absolute values
                             in thou, reletive to the workpiece sholder.
        :return: An array containing the commands for a single complete piece.
        """
        command_list = []


        command_list.extend(self.generate_start_code())
        for i in range(0, self._num_pins):
            depth = self._workpiece_height - well_heights[i]
            if i == 0:
                spacing = well_spacings[i]
            else:
                spacing = well_spacings[i] - well_spacings[i-1]
            command_list.extend(self.generate_pin_codes(depth,
                                                        well_angles[i],
                                                        spacing))

        command_list.extend(self.generate_end_code())

        return command_list

    def generate_start_code(self):
        """
        :return:
        """
        command_list = []
        command_list.append(self._pause_execution()) #start program record
        command_list.append(self._set_relative())
        return command_list

    def generate_end_code(self):
        """
        :return:
        """
        command_list = []
        command_list.append(self._resume_execution()) #start program run
        return command_list

    def generate_pin_codes(self, depth, angle, spacing):
        """
        This function returns a list of the G-Codes required to cut the well
        specified.
        Pre-Condition: _safe_z directly above the pervious well with the angle
                       at zero.
        Post-Condition: _save_z directly above current well with angle at zero
                        and the current well cut.
        :param depth: How deep to make the well in thou
        :param angle: how much to turn the A axis in degrees eletive to X
                      perpendicular to the plane of the cutting disk.
        :param spacing: How far along the work-piece the current well is from
                        the previous well or safe location.
        :return: Above specified list of G code commands.
        """
        command_list = []
        # Positioned above pervious well at safe height. A = 0
        command_list.append(self._controlled_move(spacing,
                                                  0,
                                                  angle,
                                                  self._feed_rate_move))
        # Positioned above current well at safe height. A = Angle
        command_list.append(self._controlled_move(0,
                                                  - self._safe_Z + self._workpiece_height,
                                                  0,
                                                  self._feed_rate_move))
        # Positioned above current well at top of piece. A = Angle.
        command_list.append(self._controlled_move(0,
                                                  -1 * depth,
                                                  0,
                                                  self._feed_rate_cut))
        # Positioned at the bottom of the well. A = Angle.
        command_list.append(self._controlled_move(0,
                                                  self._safe_Z - (self._workpiece_height - depth),
                                                  0,
                                                  self._feed_rate_move))
        # Positioned at _safe_z above well. A = Angle.
        command_list.append(self._controlled_move(0,
                                                  0,
                                                  -1 * angle,
                                                  self._feed_rate_move))
        # Positioned at _safe_z above well. A = 0.
        return command_list

    def _controlled_move(self, X = 0, Z = 0, A = 0, F = _feed_rate_cut):
        """
        Generates a string corresponding to a single G1 command. This function
        implicitly assumes that the device is in reletive mode, and will thus
        default x, z, and a values to 0 if no value is provided.
        :param X:
        :param Z:
        :param A:
        :param F:
        :return:
        """
        return "G1,X{0},Z{1},A{2},F{3}".format(X, Z, A, F)

    def _pause_execution(self):
        """
        Generates the string corresponding to a pause execution command. Once
        sent the command queue will immediately stop being executed (but will not
        be cleared)
        :return: string
        """
        return "M123"

    def _resume_execution(self):
        """
        Generates the string corresponding to a resume execution command. The
        command queue will resume execution imediatly upon recipt of this
        command.
        :return: string
        """
        return "M124"

    def _set_absolute(self):
        """
        generates the string corresponding to a set absolute command. This will
        be added be added to the command queue, and, when executed, will cause
        subsequent move commands to be interpreted reletive to machine zero.
        :return: String
        """
        return "G90"

    def _set_relative(self):
        """
        generates the string corresponding to a set relative command. This will
        be added to the command queue and, when executed, will cause subsequnet
        move commands to be interpreted relative to the machine position when
        they are executed.
        :return:
        """
        return "G91"

class keyway ():
    def __init__(self,):
        self.paramiters = {}
        self.spacings = {}
        self.well_codes = {}
        self.well_code_length = 0

    def return_defaults(self):
        """
        :return:
        """
        pass

    def return_from_code(self, code):
        """
        Takes the input code and returns the corresponding set of depths,
        spacings and angles.

        If an invalid code block is found an error will be thrown which should
        be caught internally.
        :param code:
        :return:
        """
        