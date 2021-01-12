#!/usr/bin/python

import tkinter as tk
import serial  # baud 9600
import serial.tools.list_ports  # for listing comports

comports = [comport.device for comport in serial.tools.list_ports.comports()]

BANK_NO = 10  # number of banks
BANK_NAME_LEN = 14  # number of characters in each bank name
banknames = []  # bank names to be displayed
read_banknames = []  # bank names read from arduino

# port choice window -----------------------------------------------------------

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
# print(choice_port.get())


with serial.Serial(port=choice_port.get(), baudrate=9600, timeout=10) as ser:

    # popup window indicating to select PC mode on arduino
    window_popup_wait = tk.Tk()
    name_lbl = tk.Label(text="Select 'Connect PC' on arduino")
    name_lbl.pack(padx=15, pady=15)

    while not ser.in_waiting:
        # update window without blocking (instead of mainloop())
        window_popup_wait.update_idletasks()
        window_popup_wait.update()
    else:
        window_popup_wait.destroy()  # close the popup window

        # receive data from arduino
        while ser.in_waiting:
            line = ser.readline()
            line = line.decode()[:-2]  # delete \r\n
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
for i in range(BANK_NO): banknames[i].grid(padx=5, pady=5, column=0, row=i+1)
button_saveto_arduino.grid(padx=7, column=1, row=BANK_NO-2)
button_saveto_txt.grid(padx=7, column=1, row=BANK_NO-1)
button_load_preset.grid(padx=7, column=1, row=BANK_NO)

window_main.mainloop()
