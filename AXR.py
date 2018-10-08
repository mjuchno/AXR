# -*- coding: utf-8 -*-
"""
@author: mjuchno
"""

#%%
import sys
import os, time
import keyboard

import serial
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM6' # change to a correcto port
ser.timeout = 0

ser.open()
print("Serial com open:", ser.is_open)

log = r'e:\morto\Documents\Egosoft\X Rebirth\debuglog.txt' # change to your folder
log_str = 'AXR'

last_size = 0
now_size = 0

last_time = time.clock()
delta_time = 100e-03 # 100 ms

bn = 'tab' # keyboard button

try:
    while 1:
        now_time = time.clock()
        if now_time-last_time > delta_time:
            last_time = now_time
            if os.path.isfile(log):
                now_size = os.path.getsize(log)
            if now_size != last_size and now_size > 1000:
                last_size = now_size
                with open(log, "rb") as f:
                    f.seek(-2, os.SEEK_END)     # Jump to the second last byte.
                    while f.read(1) != b"\n":   # Until EOL is found...
                        f.seek(-2, os.SEEK_CUR) # ...jump back the read byte plus one more.
                    last = f.readline()         # Read last line.
        
                if log_str in last.decode():
                    last = last.decode().strip('\r\n')
#                    print(last)
                    SerialCmd = last[last.index(' AXR '):].split()
                    for cmd in SerialCmd:
                        ser.write((cmd+'\n').encode())
                    
        r = ser.read(1000)
        if len(r) > 0:
            response = r.decode().strip('\r\n').split('\r\n')
            for res in response:
                if 'BN' in res:
                    print('<-',res)
                    if 'ON' in res:
                        keyboard.press(bn)
                    else:
                        keyboard.release(bn)
                else:
                    print('->',res.replace('\n', ' '))

except KeyboardInterrupt:
    ser.close()
    print("Serial com open:", ser.is_open)
    if keyboard.is_pressed(bn):
        keyboard.release(bn)
    sys.exit('Program stopped')
