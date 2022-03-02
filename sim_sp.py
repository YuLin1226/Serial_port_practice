#! /usr/bin/env python
#coding=utf-8
import pty
import os
import select

def mkpty():
    master1, slave = pty.openpty()
    slaveName1 = os.ttyname(slave)
    master2, slave = pty.openpty()
    slaveName2 = os.ttyname(slave)

    print("master1's slave (slave1): ", slaveName1)
    print("master2's slave (slave2): ", slaveName2)

    return master1, master2

if __name__ == "__main__":

    master1, master2 = mkpty()
    # masterName1 = os.ttyname(master1)
    # masterName2 = os.ttyname(master2)
    print("master1: ", master1)
    print("master2: ", master2)

    while True:
        rl, wl, el = select.select([master1,master2], [], [], 1)
        for master in rl:
            print(master)
            data = os.read(master, 128)
            if master==master1:
                os.write(master2, data)
            else:
                os.write(master1, data)
