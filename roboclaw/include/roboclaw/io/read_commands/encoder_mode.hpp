#pragma once
#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.hpp"
#include "../io.hpp"
#include "../types.hpp"

namespace roboclaw
{
namespace io
{
namespace read_commands
{
struct encoder_mode
{
    using return_type = roboclaw::io::encoder_mode_1_2;
    static constexpr uint8_t CMD = 91;
    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str);
};

std::string get_string(const encoder_mode::return_type& m);

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
