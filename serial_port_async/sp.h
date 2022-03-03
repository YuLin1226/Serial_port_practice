#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost::asio;

namespace SP
{
    class SP
    {
        protected:
            std::shared_ptr<io_service> sp_service;
            std::shared_ptr<serial_port> sp_port;
            std::shared_ptr<boost::mutex> sp_mutex;
            std::shared_ptr<boost::mutex> sp_func_mutex;
            std::string sp_serial_port;
            int sp_baud_rate;
            
        public:
            SP(const std::string _serial_port, const int _baud_rate);
            virtual ~SP();

        public:
            void set_param_serial(const std::string _serial_port, const int _baud_rate);
            int open();
            int close();
            void write(std::vector<char> _data);
            void write_data(uint16_t _data);
            std::vector<char> asyncRead(size_t min_rcv);
            void readCallback(deadline_timer &timeout, const boost::system::error_code &error, std::size_t bytes_transferred);
            void timeoutCallback(serial_port &s_port, const boost::system::error_code &error);
            std::vector<char> read_and_write(uint16_t _DATA, int num_bytes);

        protected:
            bool sp_is_read_timeout;
            bool sp_available;
            std::shared_ptr<deadline_timer> sp_timeout;
            std::mutex sp_std_mutex;
    };
}