#include "sp.h"
#include <iostream>
#include <thread>
#include <boost/thread/mutex.hpp>

std::shared_ptr<SP::SP> sp{ new SP::SP{"/dev/pts/6", 115200} };

int main(int argc, char **argv)
{
    sp->open();
    uint16_t _DATA = 0x0001;

    while(true){
        try
        {
            std::vector<char> rcv = sp->read_and_write(_DATA, 10);
            for(auto i=0; i<rcv.size(); i++){
                std::cout << std::hex << rcv[i] << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        sleep(3);
    }
    sp->close();
    return 0;
}

