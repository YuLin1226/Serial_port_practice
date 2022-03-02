
#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <numeric>
#include <boost/asio.hpp>
#include <iomanip>
#include <boost/bind.hpp>
#include <bitset>

union unionType
    {
        uint8_t data_byte_[2];
        uint16_t data_;
    };



void single_register_write(boost::asio::serial_port *serialPort)
    {
        uint8_t _id{0x01}; 
        uint8_t _function_code{0x03}; 
        uint16_t _addr{0x1A01};
        uint16_t _data{0x001c};

        // 建立資料的空陣列
        std::vector<uint8_t> p_data;
        p_data.clear();
        // 存入 _id, _func, _addr, _data 等資訊
        p_data.push_back(_id);
        p_data.push_back(_function_code);
        p_data.push_back(_addr);        // 先低位
        p_data.push_back(_addr >> 8);   // 再高位
        p_data.push_back(_data);
        p_data.push_back(_data >> 8);
        
        // for(int i=0; i<p_data.size(); i++){
        //     std::cout << "send data: " << p_data[i] << std::endl;
        // }

        // std::bitset<8> x(_addr);
        // std::bitset<8> y(_addr>>8);
        // std::cout << "data(binary): " << y <<" "<< x << std::endl;

        std::vector<char> p_char(p_data.begin(), p_data.end());
        for(auto i=0;i<p_char.size();i++){
            printf("p_char[%i] = ", i);
            std::bitset<8> x(p_char[i]);
            std::cout << x << std::endl;
        }


        // std::cout << "p_char size: " << p_char.size() << std::endl;
        // std::cout << "p_char[] " << p_char[0] << std::endl;
        // std::cout << "p_char[1]: " << p_char[1] << std::endl;
        // std::bitset<8> x(p_char[0]);
        // std::bitset<8> y(p_char[1]);
        // std::cout << "send data(binary): " << y <<" "<< x << std::endl;

        while(1){

            // unionType d;
            // for(auto i=0;i<2;i++){
            //     d.data_byte_[i] = p_char[i];
            // }
            // std::cout << "send data: ";
            // for(auto i=0; i<p_char.size(); i++){
            //     std::cout << std::hex << p_char[i] << " ";
            // }
            // std::cout << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            boost::system::error_code error;
            size_t size = serialPort->write_some(boost::asio::buffer(p_char, sizeof(p_char)), error);
            if (error) {
                throw boost::system::system_error(error);
            }
            // std::cout << "write size: " << size << std::endl;
            if(p_char.size() != size) {
                throw "Write Size Error!!!";
            }  
        }


    }

void modbus_read(boost::asio::serial_port *serialPort) {
        try
        {
            const int max_char{10};
            size_t total_size = 0;
            std::vector<char> p_char;
            while(true)
            {
                p_char.clear();
                char buff[max_char]{ 0 };
                auto read_size = serialPort->read_some(boost::asio::buffer(buff, max_char));

                if(read_size > 0)
                {
                    std::vector<char> p_char_tmp(buff, buff+read_size);
                    p_char.insert(p_char.end(), p_char_tmp.begin(), p_char_tmp.end());
                    
                    for(auto i=0;i<p_char.size();i++){
                        printf("p_char[%i] = ", i);
                        std::bitset<8> x(p_char[i]);
                        std::cout << x << std::endl;
                    }

                    std::cout << std::endl << "===================" << std::endl;
                }
                else 
                {
                    usleep(1000);
                    continue;
                }                
            }  
            throw "Serial Port Is Not Open";
        }
        catch (const std::exception& ex)
        {
            std::cout << "Read exception " << ex.what() << std::endl;
        }
    }




void sendThreadRun(boost::asio::serial_port *serialPort)
{
    char sendBuffer[38];
    int testCount = 0;

    boost::system::error_code errorCode;

    while(true)
    {
        memset(sendBuffer,0,sizeof(sendBuffer));

        testCount++;
        sprintf(sendBuffer,"serialport send test , test count %d\n",testCount);

        printf("%s\n",sendBuffer);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        serialPort->write_some(boost::asio::buffer(sendBuffer,sizeof(sendBuffer)));
    }
}

void receiveThreadRun(boost::asio::serial_port *serialPort)
{
    char receiveBuffer[1024]{0};
    int readCount = 0;

    while(true)
    {
        memset(receiveBuffer,0,sizeof(receiveBuffer));

        readCount = serialPort->read_some(boost::asio::buffer(receiveBuffer,sizeof(receiveBuffer)));
        if(readCount)
        {
            printf("[received] %s\n",receiveBuffer);
            readCount = 0;
        }
    }
}

int main(int argc, char **argv)
{
    printf("============boost serialport demo===============\n");

    boost::asio::io_service ioService;
    boost::asio::serial_port serialPort(ioService, "/dev/pts/6");

    serialPort.set_option(boost::asio::serial_port::baud_rate(9600));
    serialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
    serialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
    serialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
    serialPort.set_option(boost::asio::serial_port::character_size(8));

    boost::asio::serial_port serialPort1(ioService, "/dev/pts/8");

    serialPort1.set_option(boost::asio::serial_port::baud_rate(9600));
    serialPort1.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
    serialPort1.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
    serialPort1.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
    serialPort1.set_option(boost::asio::serial_port::character_size(8));
    // std::thread sendThread(sendThreadRun,&serialPort);
    // std::thread receiveThread(receiveThreadRun,&serialPort1);

    std::thread sendThread(single_register_write, &serialPort);
    std::thread receiveThread(modbus_read,&serialPort1);
    
    sendThread.join();
    receiveThread.join();

    return 0;
}
