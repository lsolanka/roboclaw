#pragma once

#include <boost/asio.hpp>
#include <boost/core/demangle.hpp>
#include <boost/endian/conversion.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>

#include <roboclaw/io/crc_calculator.hpp>
#include <roboclaw/logging.hpp>

namespace roboclaw
{
namespace io
{
inline uint16_t read_crc(boost::asio::serial_port& port)
{
    uint16_t value;
    boost::asio::read(port, boost::asio::buffer(&value, 2));
    return boost::endian::big_to_native(value);
}

inline void write_crc(boost::asio::serial_port& port, uint16_t crc)
{
    boost::endian::native_to_big_inplace(crc);
    boost::asio::write(port, boost::asio::buffer(&crc, 2));
}

template<typename T>
T read_value(boost::asio::serial_port& port, crc_calculator_16& crc,
             boost::log::record_ostream& strm)
{
    static_assert(std::is_integral<T>(), "read_value() can only process integral types");
    T value;
    boost::asio::read(port, boost::asio::buffer(&value, sizeof(T)));
    crc << value;
    boost::endian::big_to_native_inplace(value);
    strm << " " << +value;
    return value;
}

template<typename T>
void write_value(T value, boost::asio::serial_port& port, crc_calculator_16& crc,
                 boost::log::record_ostream& strm)
{
    static_assert(std::is_integral<T>(), "write_value() can only process integral types");
    strm << " " << +value;
    boost::endian::native_to_big_inplace(value);
    boost::asio::write(port, boost::asio::buffer(&value, sizeof(T)));
    crc << value;
}

class serial_controller
{
  public:
    serial_controller(const std::string& port_name, uint8_t address)
        : lg(logger::get()), address(address), port(io_service)
    {
        boost::system::error_code ec = port.open(port_name, ec);
        if (!port.is_open())
        {
            throw std::runtime_error(std::string("Could not open serial port: ") +
                                     port_name + ": " + ec.message());
        }
    }

    uint8_t get_address() const { return address; }

    boost::asio::serial_port& get_port() { return port; }

    template<typename command>
    typename command::return_type read()
    {
        auto rec = lg.open_record(boost::log::keywords::severity = debug);
        boost::log::record_ostream strm;
        if (rec)
        {
            strm.attach_record(rec);
        }

        crc_calculator_16 calculated_crc;
        calculated_crc << get_address() << uint8_t(command::CMD);

        send_command<command>(strm);

        strm << "; recv: ";
        typename command::return_type result =
                command::read_response(get_port(), calculated_crc, strm);
        uint16_t received_crc = read_crc(get_port());

        strm << " " << std::hex << std::showbase << received_crc;
        strm.flush();
        if (rec) lg.push_record(std::move(rec));

        if (calculated_crc.get() != received_crc)
        {
            using boost::core::demangle;
            std::stringstream ss;
            ss << "received crc=" << std::hex << received_crc << " does not match "
               << "calculated crc=" << std::hex << calculated_crc.get() << " for command "
               << +uint8_t(command::CMD) << " (" << demangle(typeid(command).name())
               << ")";
            throw std::runtime_error(ss.str());
        }

        return result;
    }

    template<typename command>
    bool write(const command& cmd)
    {
        auto rec = lg.open_record(boost::log::keywords::severity = debug);
        boost::log::record_ostream strm;
        if (rec)
        {
            strm.attach_record(rec);
        }

        send_command<command>(strm);

        crc_calculator_16 calculated_crc;
        calculated_crc << get_address() << uint8_t(command::CMD);

        cmd.write(get_port(), calculated_crc, strm);
        write_crc(get_port(), calculated_crc.get());
        strm << " " << std::hex << std::showbase << calculated_crc.get();

        uint8_t response;
        boost::asio::read(port, boost::asio::buffer(&response, 1));
        strm << "; recv: " << +response;

        strm.flush();
        if (rec) lg.push_record(std::move(rec));

        return response == 0xff;
    }

    ~serial_controller() { port.close(); }

  private:
    logger_t& lg;

    uint8_t address;
    boost::asio::io_service io_service;
    boost::asio::serial_port port;

    template<typename command>
    void send_command(boost::log::record_ostream& strm)
    {
        uint8_t buffer[2] = {address, command::CMD};
        boost::asio::write(port, boost::asio::buffer(buffer, 2));

        strm << "serial send: " << std::hex << std::showbase << +address << " "
             << std::dec << +uint8_t(command::CMD);
    }
};

}  // namespace io
}  // namespace roboclaw
