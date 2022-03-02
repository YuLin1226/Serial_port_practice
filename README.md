# Serial_port_practice
###### tags: `SerialPort`,`Ubuntu`,`python`,`cpp`

[TOC]



## 使用
### 建立串口（必須）
先依照此程式建立虛擬串口：```simulate_serial_port.py```

＊使用：```$ python3 simulate_serial_port.py```

### 通訊（Python）

分別建立下列兩個檔案，
讀串口資料：```serial_port_read.py``` 
寫串口資料：```serial_port_write.py```


＊使用：```$ python3 serial_port_read.py``` 和 ```$ python3 serial_port_write.py```

> 備註：讀寫兩個檔案我都有加上權限 
> ```$ chmod +x serial_port_read.py```
> ```$ chmod +x serial_port_write.py```
> 但我不知道有沒有差。



### 通訊（Cpp）
建立以下檔案，這是一個自收自發的程式：```main.cpp```
編譯：```$ g++ main.cpp -o main -lpthread```
執行：```$ ./main```


## 參考連結

[Ubuntu 模拟串口通信](https://cloud.tencent.com/developer/article/1744848)

[Ubuntu 下串口模拟工具的使用_亅会飞的乌龟的博客-程序员信息网_ubuntu 虚拟串口](https://www.i4k.xyz/article/a844651990/80624019)

[Ubuntu 下使用虛擬串口進行開發測試](https://www.zendei.com/article/55747.html)

[解决 ImportError: No module named 'serial' 问题](https://blog.csdn.net/jiangchao3392/article/details/75107330)

[Access Serial port with Python on Raspberry Pi3](https://raspberrypi.stackexchange.com/questions/59741/access-serial-port-with-python-on-raspberry-pi3)

[WzSerialPort's github](https://github.com/ayowin/WzSerialPort)
