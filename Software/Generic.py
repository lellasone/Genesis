import Tkinter as tk
import tkFileDialog
import os

class generic():
    _num_pins = 7
    _feed_rate_move = 1 #thou/second non-contact moves.
    _feed_rate_cut = 2    #thou/second contact moves.
    _safe_Z = 1 #Given in thou, reletive to the base of the workpiece. Should
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
        self._backing_height = tk.IntVar()
        self._backing_num_pins = tk.IntVar()
        self._backing_num_pins.set(self._num_pins)
        self._backing_type = tk.StringVar()  #Stores the current model metal stock.
        self._backing_type.set("default")
        self._backing_code = tk.StringVar()
        self._backing_code.set(" ")


        self.set_lists_default()
        self._backing_height.set(0)
        pass

    def return_gui_frame(self):
        subframe = tk.Frame()
        title = tk.Label(subframe, text = "Workpiece Type: 'Generic, {0} pin'".format(self._num_pins))
        pins = self._return_pins_frame(subframe)
        params = self._paramiters_frame(subframe)
        basic = self._return_basic_frame(subframe)

        basic.grid(row = 0, column = 0, columnspan = 1, rowspan = 2)
        title.grid(row = 0, column = 0, columnspan = 2)
        pins.grid(row = 1, column = 1, columnspan = 2)
        params.grid(row = 1, column = 3, sticky = 'W')
        return subframe

    def _return_basic_frame(self, master):
        subframe = tk.Frame(master)
        stock_label = tk.Label(subframe, text = "Type: ")
        stock = tk.OptionMenu(subframe, self._backing_type, "default", "test")
        stock.config(width = 12)
        code = self._return_label_entry(subframe,
                                        "Code: ",
                                        self._backing_code,
                                        width = 18)
        save = tk.Button(subframe, text = "Save",
                         width = 20,
                         command = self._save_code)
        run = tk.Button(subframe,
                        text = "Run",
                        width = 20)


        stock_label.grid(row = 0, column = 0)
        stock.grid(row = 0, column = 1, columnspan = 2)
        code.grid(row = 1, column = 0, columnspan = 2)
        save.grid(row = 2, column = 0, columnspan = 2)
        run.grid(row = 3, column = 0, columnspan = 2)
        return subframe

    def _paramiters_frame(self, master):
        subframe = tk.Frame(master)
        height = self._return_label_entry(subframe,
                                          "Stock Height",
                                          self._backing_height)
        num_pins = self._return_label_entry(subframe,
                                            "# Pins",
                                            self._backing_num_pins)


        height.grid(row = 0, column = 0)
        num_pins.grid(row = 0, column = 1)

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

    def set_lists_default(self):
        self._backing_well_angles = []
        self._backing_well_depths = []
        self._backing_well_spacings = []

        self._backing_well_angles = self.generate_list(self._backing_well_angles,
                                                       self._num_pins,
                                                       self._DEFAULT_ANGLE)

        self._backing_well_spacings = self.generate_list(self._backing_well_spacings,
                                                         self._num_pins,
                                                         self._DEFAULT_SPACING)

        self._backing_well_depths = self.generate_list(self._backing_well_depths,
                                                       self._num_pins,
                                                       self._DEFAULT_DEPTH)

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
