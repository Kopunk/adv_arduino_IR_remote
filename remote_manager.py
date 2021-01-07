#!/usr/bin/python

import tkinter as tk
import serial  # baud 9600
import serial.tools.list_ports  # for listing comports

comports = [comport.device for comport in serial.tools.list_ports.comports()]

BANK_NO = 8  # number of banks
BANK_NAME_LEN = 14  # number of characters in each bank name
banknames = []  # bank names to be displayed
read_banknames = []  # bank names read from arduino

# port choice window -----------------------------------------------------------
# port choice_port functions
def click_confirm_port():
    window_port.destroy()

# window_port instance
window_port = tk.Tk()
window_port.title("Adv IR Remote Manager")

# adding widgets
name_lbl = tk.Label(text="Choose USB port")

choice_port = tk.StringVar()  # result after destroying the window
choice_port.set(comports[0])
combobox_port = tk.OptionMenu(window_port, choice_port, *comports)
button_confirm_port = tk.Button(window_port, text="Confirm", command=window_port.destroy)

# stack widgets
name_lbl.pack(pady=10)
combobox_port.pack(pady=10)
button_confirm_port.pack(pady=10)

window_port.mainloop()
print(choice_port.get())

with serial.Serial(port="COM5", baudrate=9600, timeout=3) as ser:

    while not ser.in_waiting:
        pass
    else:
        while ser.in_waiting:
            line = ser.readline()
            line = line.decode()[:-2]  # delete \n\r
            read_banknames.append(line)


# window_main instance ---------------------------------------------------------

window_main = tk.Tk()
window_main.title("Adv IR Remote Manager")

# button functions

def click_saveto_arduino():
    pass

def click_saveto_txt():
    pass

def click_load_preset():
    pass

# adding wdgets

name_lbl = tk.Label(text="Bank name")

banknames = [tk.Entry(window_main, width=15) for i in range(BANK_NO)]
for i in range(BANK_NO): banknames[i].insert(index=0, string=read_banknames[i])

button_saveto_arduino = tk.Button(text="Save to Arduino")
button_saveto_txt = tk.Button(text="Save to text file")
button_load_preset = tk.Button(text="Load file")

# grid widgets
name_lbl.grid(column=0, row=0, pady=10)
for i in range(BANK_NO): banknames[i].grid(column=0, row=i+1)
button_saveto_arduino.grid(column=1, row=BANK_NO-2)
button_saveto_txt.grid(column=1, row=BANK_NO-1)
button_load_preset.grid(column=1, row=BANK_NO)

window_main.mainloop()
