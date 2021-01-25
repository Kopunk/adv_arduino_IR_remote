#!/usr/bin/python

import tkinter as tk
import serial  # baud 9600
import serial.tools.list_ports  # for listing comports
from time import sleep
import os
THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

comports = [comport.device for comport in serial.tools.list_ports.comports()]

BANK_NO = 10  # number of banks
BANK_NAME_LEN = 14  # number of characters in each bank name
SEQUENCE_LEN = 4 * BANK_NO  # total number of signals in all sequences
BUTTON_NO = 15  # total number of buttons(' signals)
banknames = []  # bank names to be displayed
read_banknames = []  # bank names read from arduino



is_write_to_EEPROM = 0  # sent to arduino to inicate whether EEPROM will be updated

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

# open serial connection
# with serial.Serial(port=choice_port.get(), baudrate=9600, timeout=10) as ser:
ser = serial.Serial(port=choice_port.get(), baudrate=9600)

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
    with open(os.path.join(THIS_FOLDER, 'temp'), "w") as f:
        for i in range(BUTTON_NO):
            while not ser.in_waiting: pass
            readln = ser.readline()
            readln = readln.decode()
            readln = readln[:-2] if readln[-2:] == "\r\n" else readln
            f.write(f"{readln}\n")
            print(readln)
            #sleep(.01)

        for i in range(BANK_NO):
            while not ser.in_waiting: pass
            readln = ser.readline()
            readln = readln.decode()
            readln = readln[:-2] if readln[-2:] == "\r\n" else readln
            f.write(f"{readln}\n")
            print(readln)
            #sleep(.01)
            read_banknames.append(readln)  # to display banknames

        for i in range(SEQUENCE_LEN):
            while not ser.in_waiting: pass
            readln = ser.readline()
            readln = readln.decode()
            readln = readln[:-2] if readln[-2:] == "\r\n" else readln
            f.write(f"{readln}\n")
            print(readln)
            #sleep(.01)

        assert not ser.in_waiting, "Too many bytes in buffer."

        # ser.write((2).to_bytes(length=1, byteorder='little'))


# window_main instance ---------------------------------------------------------

window_main = tk.Tk()
window_main.title("Adv IR Remote Manager")

# button functions

def write_to_arduino(file_name):
    ser.write((1).to_bytes(length=1, byteorder='little'))
    with open(os.path.join(THIS_FOLDER, file_name), "r") as f_write:
        ser.read()
        print("write signals")
        for i in range(BUTTON_NO):
            x = str(f_write.readline())
            while x[-1] == "\n" or x[-1] == "\r":
                x = x[:-1]
            assert len(x) <= 11, "Sequence too big"
            print(x)
            ser.write(x.encode())
            while not ser.in_waiting : sleep(.1)
            else: ser.read()

            
            # x = int(x).to_bytes(length=4, byteorder="little", signed=True)
            # for y in x:
            #     print(y)
            #     ser.write(y)

        print("write bankno")
        for i in range(BANK_NO):
            x = str(f_write.readline())
            while x[-1] == "\n" or x[-1] == "\r" or len(x) > 14:
                x = x[:-1]
            print(x)
            ser.write(x.encode())
            sleep
            while not ser.in_waiting : sleep(.1)
            else: ser.read()

        print("write seq")
        for i in range(SEQUENCE_LEN):
            x = f_write.readline()
            while x[-1] == "\n" or x[-1] == "\r":
                x = x[:-1]
            print(x)
            ser.write(x.encode())
            while not ser.in_waiting : sleep(.1)
            else: ser.read()



def click_saveto_arduino():
    with open(os.path.join(THIS_FOLDER, 'newpreset'), "w") as f_new:
        with open(os.path.join(THIS_FOLDER, 'temp'), "r") as f_temp:
            for i in range(BUTTON_NO):
                f_new.write(f"{f_temp.readline()}")  # no newline needed
            for i in range(BANK_NO):
                f_new.write(f"{banknames[i].get()}\n")
                f_temp.readline()
            for i in range(SEQUENCE_LEN):
                f_new.write(f"{f_temp.readline()}")  # no newline needed
    write_to_arduino("newpreset")


def click_load_preset():
    file_name = entry_file_name.get()
    try:
        with open(os.path.join(THIS_FOLDER, file_name), "r") as f:
            pass
        write_to_arduino(file_name)
    except:
        write_to_arduino("temp")

# adding widgets

name_lbl = tk.Label(text="Bank name")

label_file_name = tk.Label(text="Preset file name")

entry_file_name = tk.Entry(window_main, width=15)
entry_file_name.insert(index=0, string="preset.txt")

# add all bankname entry spaces at once
banknames = [tk.Entry(window_main, width=15) for i in range(BANK_NO)]
for i in range(BANK_NO): 
    banknames[i].insert(index=0, string=read_banknames[i])

button_saveto_arduino = tk.Button(text="Save to Arduino", command=click_saveto_arduino)
button_load_preset = tk.Button(text="Load preset", command=click_load_preset)

# grid widgets
name_lbl.grid(column=0, row=0, pady=3)
for i in range(BANK_NO): 
    banknames[i].grid(padx=5, pady=5, column=0, row=i+1)
button_saveto_arduino.grid(padx=7, column=1, row=BANK_NO-1)
button_load_preset.grid(padx=7, column=1, row=BANK_NO)

label_file_name.grid(column=1, row=0, pady=3)
entry_file_name.grid(padx=5, pady=5, column=1, row=1)

window_main.mainloop()

ser.write((2).to_bytes(length=1, byteorder='little'))
ser.close()
