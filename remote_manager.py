#!/usr/bin/python

import tkinter as tk
import serial  # baud 9600

BANK_NO = 8  # number of banks
BANK_NAME_LEN = 14  # number of characters in each bank name
banknames = []  # names of banks
savebuttons = []  # 

readbytes = "Bank#1        Bank#2        Bank#3  "


# with serial.Serial() as ser:
#     ser.baudrate = 9600
#     ser.port = "COM5"
#     ser.open()
#     readbytes = ser.read(BANK_NO*BANK_NAME_LEN)




window = tk.Tk()
window.title("adv IR remote manager")

# adding wdgets

name_lbl = tk.Label(text="Bank name")

banknames = [tk.Entry() for i in range(BANK_NO)]
for i in range(BANK_NO):
    banknames[i].insert(index=0, string=readbytes[i*BANK_NAME_LEN : (i+1)*BANK_NAME_LEN])

savebuttons = [tk.Button(text="Save") for i in range(BANK_NO)]

# grid widgets
name_lbl.grid(column=0, row=0)
for i in range(BANK_NO): banknames[i].grid(column=0, row=i+1)
for i in range(BANK_NO): savebuttons[i].grid(column=1, row=i+1)


window.mainloop()