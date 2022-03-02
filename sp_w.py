#usr/bin/python3
# -*- coding: utf-8 -*-

import serial
ser=serial.Serial("/dev/pts/2",9600,timeout=0.5)

print("serial_name:",ser.name)
ser.close()
ser.open()
print(ser.isOpen())

# while True:

ser.write(b"hello") # 二进制 

ser.close()#关闭端口