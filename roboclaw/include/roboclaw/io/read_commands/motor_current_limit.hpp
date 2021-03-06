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
template<uint8_t command_id>
struct motor_current_limit
{
    struct return_type
    {
        units::quantity<units::current> min;
        units::quantity<units::current> max;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str)
    {
        return_type r;

        r.max = read_value<uint32_t>(port, crc, log_str) / 100.f * units::amperes;
        r.min = read_value<uint32_t>(port, crc, log_str) / 100.f * units::amperes;

        return r;
    }
};
using m1_current_limit = motor_current_limit<135>;
using m2_current_limit = motor_current_limit<136>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
