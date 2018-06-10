#pragma once
#include <boost/asio.hpp>
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
struct motor_pwm_values
{
    struct return_type
    {
        float m1;
        float m2;
    };
    static constexpr uint8_t CMD = 48;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str);
};

std::string get_string(const motor_pwm_values::return_type& v);

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
