import Tkinter as tk
import Generic

class main_gui:
    def __init__(self, master):
        self.manual_x_distance = tk.IntVar()
        self.manual_x_distance.set(5) #default move distance, in thou
        self.manual_z_distance = tk.IntVar()
        self.manual_z_distance.set(5) #default move distance, in thou
        self.manual_a_distance = tk.IntVar()
        self.manual_a_distance.set(5) #default move distance, in degrees
        self.manual_speed = tk.IntVar()
        self.manual_speed.set(5) #default move distance, in thou

        self.workpiece_script = Generic.generic() #stores the workpiece object.
        self.workpiece_frame = self.workpiece_script.return_gui_frame()  #stores the workpiece GUI.

        self.master = master
        menu = self._return_menu_bar(master)
        master.config(menu = menu)

        manual = self._return_manual_control(self.master)
        spacer = tk.Frame(bd = 4, relief = "ridge", width = 6, padx = 30, height = 6 * 20)


        manual.grid(row = 1, column = 0)
        spacer.grid(row = 1, column = 1)
        self.workpiece_frame.grid(row = 1, column = 2)

    def return_workpiece_script(self, type):
        return Generic.generic()

    def _return_manual_control(self, master):
        sub_frame = tk.Frame(master)
        # X axis.
        label_x = tk.Label(sub_frame, text = "X Axis")
        increment_x = tk.Button(sub_frame, text = "Incriment")
        decrement_x = tk.Button(sub_frame, text = "Decriment")
        amount_x = tk.Entry(sub_frame, textvariable = self.manual_x_distance)
        amount_x.config(width = 10)

        # Z axis
        label_z = tk.Label(sub_frame, text = "Z Axis")
        increment_z = tk.Button(sub_frame, text = "Incriment")
        decrement_z = tk.Button(sub_frame, text = "Decriment")
        amount_z = tk.Entry(sub_frame, textvariable = self.manual_z_distance)
        amount_z.config(width = 10)

        # Motor Speed
        label_s = tk.Label(sub_frame, text = "Speed")
        increment_s = tk.Button(sub_frame, text = "Incriment")
        decrement_s = tk.Button(sub_frame, text = "Decriment")
        amount_s = tk.Entry(sub_frame, textvariable = self.manual_speed)
        amount_s.config(width = 10)

        # Motor Speed
        label_a = tk.Label(sub_frame, text = "Angle")
        increment_a = tk.Button(sub_frame, text = "Incriment")
        decrement_a = tk.Button(sub_frame, text = "Decriment")
        amount_a = tk.Entry(sub_frame, textvariable = self.manual_speed)
        amount_a.config(width = 10)

        # Buttons
        home = tk.Button(sub_frame, text = "Home", width = 10)
        G30 = tk.Button(sub_frame, text = "G30", width = 10)
        stop = tk.Button(sub_frame, text = "STOP", width = 10)
        ping = tk.Button(sub_frame, text = "Ping", width = 10)

        spacer_1 = tk.Frame(sub_frame, height = 10)

        label_x.grid(row = 1, column = 0)
        label_z.grid(row = 1, column = 1)
        label_s.grid(row = 1, column = 2)
        label_a.grid(row = 1, column = 3)
        increment_x.grid(row = 2, column = 0)
        increment_z.grid(row = 2, column = 1)
        increment_s.grid(row = 2, column = 2)
        increment_a.grid(row = 2, column = 3)
        amount_x.grid(row = 3, column = 0)
        amount_z.grid(row = 3, column = 1)
        amount_s.grid(row = 3, column = 2)
        amount_a.grid(row = 3, column = 3)
        decrement_x.grid(row = 4, column = 0)
        decrement_z.grid(row = 4, column = 1)
        decrement_s.grid(row = 4, column = 2)
        decrement_a.grid(row = 4, column = 3)
        spacer_1.grid(row = 5, column = 0, columnspan = 5)
        home.grid(row = 6, column = 0)
        G30.grid(row = 6, column = 1)
        stop.grid(row = 6, column = 2)
        ping.grid(row = 6, column = 3)
        return sub_frame

    def _return_menu_bar(self, master):
        menu_top = tk.Menu(master)
        menu_top.add_command(label = "Help", command = self._display_help)

        return menu_top







    def _display_help(self):
        print "help"








if __name__ == "__main__":

    root = tk.Tk()
    main_gui(root)
    root.mainloop()
