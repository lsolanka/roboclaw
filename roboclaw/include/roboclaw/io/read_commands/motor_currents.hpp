#pragma once
#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.hpp"
#include "../io.hpp"

namespace roboclaw
{
namespace io
{
namespace read_commands
{
struct motor_currents
{
    struct return_type
    {
        units::quantity<units::current> m1;
        units::quantity<units::current> m2;
    };
    static constexpr uint8_t CMD = 49;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc, std::string& log_str)
    {
        return_type r;
        r.m1 = read_value<uint16_t>(port, crc, log_str) / 100.f * units::amperes;
        r.m2 = read_value<uint16_t>(port, crc, log_str) / 100.f * units::amperes;
        return r;
    }
};

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
