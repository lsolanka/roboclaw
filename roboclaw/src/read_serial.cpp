#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "read_serial <port_name>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* port_name = argv[1];

    boost::asio::io_service io_service;
    boost::asio::serial_port port(io_service);

    boost::system::error_code ec = port.open(port_name, ec);
    port.set_option(asio::serial_port_base::baud_rate(57600));

    if (!port.is_open())
    {
        throw std::runtime_error(std::string("Could not open serial port: ") + port_name +
                                 ": " + ec.message());
    }

    while (true)
    {
        char c;
        std::string result;
        while (true)
        {
            boost::asio::read(port, boost::asio::buffer(&c, 1));
            if (c != '\n')
            {
                result += c;
            }
            else
            {
                break;
            }
        }

        std::cout << result << std::endl;
    }

    return 0;
}
