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
template<uint8_t command_id>
struct motor_pid_base
{
    using return_type = roboclaw::io::pid_values;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     boost::log::record_ostream& strm);
};
using m1_velocity_pid = motor_pid_base<55>;
using m2_velocity_pid = motor_pid_base<56>;

extern template struct motor_pid_base<55>;
extern template struct motor_pid_base<56>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
