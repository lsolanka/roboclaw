#pragma once

#include <boost/asio.hpp>
#include <boost/core/demangle.hpp>
#include <boost/endian/conversion.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>
#include <optional>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

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
T read_value(boost::asio::serial_port& port, crc_calculator_16& crc, std::string& log_str)
{
    static_assert(std::is_integral<T>(), "read_value() can only process integral types");
    T value;
    boost::asio::read(port, boost::asio::buffer(&value, sizeof(T)));
    crc << value;
    boost::endian::big_to_native_inplace(value);
    log_str += fmt::format(" {:d}", value);
    return value;
}

template<typename T>
void write_value(T value, boost::asio::serial_port& port, crc_calculator_16& crc,
                 std::string& log_str)
{
    static_assert(std::is_integral<T>(), "write_value() can only process integral types");
    log_str += fmt::format(" {:d}", value);
    boost::endian::native_to_big_inplace(value);
    boost::asio::write(port, boost::asio::buffer(&value, sizeof(T)));
    crc << value;
}

class serial_controller
{
  public:
    serial_controller(const std::string& port_name, uint8_t address)
        : lg(get_roboclaw_logger()), address(address), port(io_service)
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
        std::string log_str;

        crc_calculator_16 calculated_crc;
        calculated_crc << get_address() << uint8_t(command::CMD);

        send_command<command>(log_str);

        log_str += "; recv:";
        typename command::return_type result =
                command::read_response(get_port(), calculated_crc, log_str);
        uint16_t received_crc = read_crc(get_port());

        log_str += fmt::format(" {:#x}", received_crc);
        lg->debug(log_str);

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
        std::string log_str;

        send_command<command>(log_str);

        crc_calculator_16 calculated_crc;
        calculated_crc << get_address() << uint8_t(command::CMD);

        cmd.write(get_port(), calculated_crc, log_str);
        write_crc(get_port(), calculated_crc.get());
        log_str += fmt::format(" {:#x}", calculated_crc.get());

        uint8_t response;
        {
            std::optional<boost::system::error_code> timer_result;
            boost::asio::deadline_timer timer(port.get_io_service());
            timer.expires_from_now(boost::posix_time::milliseconds{100});
            timer.async_wait([&timer_result](const auto& error) { timer_result = error; });


            std::optional<boost::system::error_code> read_result;
            boost::asio::async_read(port, boost::asio::buffer(&response, 1),
                    [&read_result](const auto& error, size_t) { read_result = error; });

            port.get_io_service().reset();
            while (port.get_io_service().run_one())
            {
                if (read_result)
                {
                    timer.cancel();
                }
                else if (timer_result)
                {
                    port.cancel();
                }
            }
            if (read_result && *read_result)
            {
                // If read but failure bail out...
                lg->error("Response read completed but with error");
                return false;
            }
            else if (!read_result)
            {
                lg->error("Response read timed out for '{}'", log_str);
                return false;
            }
        }

        lg->debug(log_str);
        return response == 0xff;
    }

    ~serial_controller() { port.close(); }

  private:
    std::shared_ptr<spdlog::logger> lg;

    uint8_t address;
    boost::asio::io_service io_service;
    boost::asio::serial_port port;

    template<typename command>
    void send_command(std::string& log_str)
    {
        uint8_t buffer[2] = {address, command::CMD};
        boost::asio::write(port, boost::asio::buffer(buffer, 2));

        log_str += fmt::format("serial send: {:x} {:d}", address, uint8_t(command::CMD));
    }
};

}  // namespace io
}  // namespace roboclaw
