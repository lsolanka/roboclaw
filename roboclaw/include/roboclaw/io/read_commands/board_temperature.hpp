#pragma once
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <sstream>
#include <string>
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
struct temperature_base
{
    using return_type = quantity<temperature>;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str)
    {
        return read_value<uint16_t>(port, crc, log_str) / 10.f * celsius::degree;
    }
};
using board_temperature_1 = temperature_base<82>;
using board_temperature_2 = temperature_base<83>;

std::string get_string(const quantity<temperature>& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
