#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <typeinfo>
#include <iomanip>
#include <boost/core/demangle.hpp>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>

#include "crc_calculator.h"

namespace roboclaw
{
namespace io
{

uint16_t read_crc(boost::asio::serial_port& port)
{
    uint16_t value;
    boost::asio::read(port, boost::asio::buffer(&value, 2));
    return boost::endian::big_to_native(value);
}

template<typename T>
T read_value(boost::asio::serial_port& port, crc_calculator_16& crc)
{
    T value;
    boost::asio::read(port, boost::asio::buffer(&value, sizeof(T)));
    crc << value;
    return boost::endian::big_to_native(value);
}


class serial_controller
{
  public:

    serial_controller(const std::string& port_name, uint8_t address)
        : address(address), port(io_service)
    {
        boost::system::error_code ec = port.open(port_name, ec);
        if (!port.is_open()) {
            throw std::runtime_error(
                std::string("Could not open serial port: ") + port_name + ": "
                      + ec.message());
        }
    }

    uint8_t get_address() const { return address; }

    boost::asio::serial_port& get_port() { return port; }

    template<typename command>
    typename command::return_type read()
    {
        crc_calculator_16 calculated_crc;
        calculated_crc << get_address() << uint8_t(command::CMD);
        
        send_read_command<command>();
        typename command::return_type result = command::read_response(get_port(), calculated_crc);

        uint16_t received_crc = read_crc(get_port());
        if (calculated_crc.get() != received_crc)
        {
            using boost::core::demangle;
            std::stringstream ss;
            ss << "received crc="<< std::hex << received_crc << " does not match "
               << "calculated crc=" << std::hex << calculated_crc.get()
               << " for command " << +uint8_t(command::CMD)
               << " (" << demangle(typeid(command).name()) << ")";
            throw std::runtime_error(ss.str());
        }

        return result;
    }

    ~serial_controller()
    {
        port.close();
    }

  private:
    uint8_t address;

    boost::asio::io_service io_service;
    boost::asio::serial_port port;

    template<typename command>
    inline void send_read_command()
    {
        uint8_t buffer[2] = {address, command::CMD};
        boost::asio::write(port, boost::asio::buffer(buffer, 2));
    }

};

}
}
