#include "sp.h"
namespace SP
{
    SP::SP(const std::string _serial_port, const int _baud_rate){
        set_param_serial(_serial_port, _baud_rate);
    }
    SP::~SP(){
        close();
    }

    void SP::set_param_serial(const std::string _serial_port, const int _baud_rate){
        sp_serial_port = _serial_port;
        sp_baud_rate = _baud_rate;
    }

    int SP::open(){
        if(sp_port != NULL){
            return -1;
        }

        if(sp_service){
            sp_port.reset();
            sp_service.reset();
        }
            
        sp_mutex = std::shared_ptr<boost::mutex>{new boost::mutex};
        sp_service = std::shared_ptr<io_service>{new io_service()};
        sp_port = std::shared_ptr<serial_port>{ new serial_port(*sp_service) };
        sp_timeout = std::shared_ptr<deadline_timer>{new deadline_timer(*sp_service)};


        try 
        {
            sp_port->open(sp_serial_port);
            sp_port->set_option(serial_port_base::baud_rate(sp_baud_rate));
            sp_port->set_option(serial_port_base::character_size(8));
            sp_port->set_option(serial_port_base::parity(serial_port_base::parity::none));
            sp_port->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        }
        catch (std::exception &ex){
            std::cout << ex.what() << std::endl;
        }
        return 0;           
    }
    int SP::close(){
        if(sp_port && sp_port->is_open()){
             sp_port->close();
        }
        return 0;           
    }
    void SP::write(std::vector<char> _data){
	    if (sp_port->is_open()){
            boost::mutex::scoped_lock lock(*sp_mutex);
            auto size = sp_port->write_some(buffer(_data));
            if(_data.size() != size){
                throw "Write Size Error!!!";
            }      
        }
        else
        {
            throw "Port not open";
        }
    }    
    void SP::write_data(uint16_t _data)
    {
        std::vector<uint8_t> sp_data;
        sp_data.clear();
        sp_data.push_back(_data >> 8);
        sp_data.push_back(_data);
        
        std::vector<char> sp_char(sp_data.begin(), sp_data.end());
        this->write(sp_char);
    }

    std::vector<char> SP::asyncRead(size_t rcv_size)
    {
        sp_service->reset();
        sp_is_read_timeout = false;
        sp_available = false;
        std::vector<char> sp_char(rcv_size);
        try
        {
            boost::mutex::scoped_lock sl(*sp_mutex);
            async_read( *sp_port, 
                        boost::asio::buffer(sp_char, rcv_size),
                        boost::bind(&SP::readCallback,
                                    this,
                                    boost::ref(*sp_timeout),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred
                                    )
                        );
            sp_timeout->expires_from_now(boost::posix_time::millisec(1500));
            sp_timeout->async_wait(boost::bind(  &SP::timeoutCallback, 
                                                this,
                                                boost::ref(*sp_port),
                                                boost::asio::placeholders::error
                                                )
                                );
            sp_service->run(); 
            
        }
        catch(const std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }
        if (sp_available)
        {
            return sp_char;
        }
        else
        {
            throw "Serial port reading timeout";
        }
    }

    void SP::readCallback(deadline_timer &timeout, const boost::system::error_code &error, std::size_t bytes_transferred)
    {
        if (error)
        {
            sp_available = false;
            std::cerr << "readCallback Error " << error << std::endl;
            return;
        }
        timeout.cancel();
        sp_available = true;
    }

    void SP::timeoutCallback(serial_port &s_port, const boost::system::error_code &error)
    {
        if (error)
        {
            return;
        }
        sp_is_read_timeout = true;
        sp_available = false;
        s_port.cancel(); 
        std::cerr << "Read timeout" << std::endl;
    }

    std::vector<char> SP::read_and_write(uint16_t _DATA, int num_bytes){
        // try
        // {
        //     write_data(_DATA);
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
        std::vector<char> response;
        {
            usleep(1500);
            try
            {
                response = asyncRead(num_bytes);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        return response;
    }

}
