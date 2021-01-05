#!/usr/bin/python

import tkinter as tk
import serial  # baud 9600

BANK_NO = 8  # number of banks
banknames = []  # names of banks
savebuttons = []  # 

readbytes = ""


with serial.Serial() as ser:
    ser.baudrate = 9600
    ser.port = "COM5"
    ser.open()
    readbytes = ser.read(BANK_NO*14)




window = tk.Tk()
window.title("adv IR remote manager")

# adding wdgets

name_lbl = tk.Label(text="Bank name")

banknames = [tk.Entry() for i in range(BANK_NO)]
for i in range(BANK_NO): banknames[i].insert(index=0, string=readbytes[] )

savebuttons = [tk.Button(text="Save") for i in range(BANK_NO)]

# grid widgets
name_lbl.grid(column=0, row=0)
for i in range(BANK_NO): banknames[i].grid(column=0, row=i+1)
for i in range(BANK_NO): savebuttons[i].grid(column=1, row=i+1)


window.mainloop()