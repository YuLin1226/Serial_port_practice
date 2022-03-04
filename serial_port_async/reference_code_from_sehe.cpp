#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <iostream>
#include <thread>

namespace net = boost::asio;
namespace ph  = std::placeholders;
using boost::system::error_code;
using namespace std::chrono_literals;

struct SerialPort {
    SerialPort(std::string const& device, unsigned baudrate) {
        _port.open(device);
        _port.set_option(net::serial_port_base::baud_rate(baudrate));
    }

    std::mutex       _mutex;
    net::io_context  _svc;
    net::serial_port _port{_svc};

    std::vector<char> read_and_write(uint16_t msg, size_t rcv_size) {
        {
            std::lock_guard   sl(_mutex);
            uint16_t data[] = {msg};
            net::write(_port, net::buffer(data));
        }
        return read(rcv_size);
    }

    std::vector<char> read(size_t rcv_size) {
        bool              available = false;
        std::vector<char> buf(rcv_size);

        std::lock_guard   sl(_mutex);

        _svc.reset();
        net::steady_timer timer{_svc, 1500ms};
        timer.async_wait([&](error_code ec) { if (!ec) _port.cancel(); });
        net::async_read(_port, net::buffer(buf), [&](error_code ec, size_t n) {
            timer.cancel();
            bool ok   = !ec || (ec == net::error::eof);
            available = ok && n;
        });

        _svc.run();

        if (not available) {
            throw std::runtime_error("Serial port timeout");
        }
        return buf;
    }
};

#include <fmt/ranges.h>
int main(int argc, char** argv) {
    SerialPort x{argc > 1 ? argv[1] : "/dev/pts/8", 115200};

    while (true) {
        try {
            fmt::print("{::#02x}\n", x.read_and_write(0x0001, 10));
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        std::this_thread::sleep_for(3s);
    }
}


/*
Refer to Sehe:
Code Website: https://godbolt.org/z/r8srb81v9
Comment Website: https://stackoverflow.com/questions/71333281/segfault-when-using-boostasioasync-read/71334268?noredirect=1#comment126098567_71334268
*/