#! /usr/bin/python3
# -*- coding: utf-8 -*-

import serial

ser=serial.Serial("/dev/pts/5",9600,timeout=1)

print("serial_name:",ser.name)
# ser.close()
# ser.open()
print(ser.isOpen())

while True:

        # s = ser.read(38) 
        s = ser.readline() #是读一行，以/n结束，要是没有/n就一直读，阻塞。
        k=0
        # for i in s:
        #         print("p_char[%i]= %s"%(k, bin(i)))
        #         k+=1
        print(s,'\n')
        # ser.close()#关闭端口
