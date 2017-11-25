#pragma once

#include <boost/asio.hpp>
#include "../units.hpp"

#include "../types.hpp"
#include "../crc_calculator.hpp"
#include "../io.hpp"

namespace roboclaw
{
namespace io
{
namespace write_commands
{

template<uint8_t command_id>
struct motor_pid_base : public roboclaw::io::pid_values
{
    static constexpr uint8_t CMD = command_id;

    motor_pid_base(float p, float i, float d, uint32_t qpps) :
        roboclaw::io::pid_values{p, i, d, qpps}
    {}

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const;
};

constexpr uint8_t M1_VELOCITY_PID_CMD = 28;
constexpr uint8_t M2_VELOCITY_PID_CMD = 29;

using m1_velocity_pid = motor_pid_base<M1_VELOCITY_PID_CMD>;
using m2_velocity_pid = motor_pid_base<M2_VELOCITY_PID_CMD>;

extern template struct motor_pid_base<M1_VELOCITY_PID_CMD>;
extern template struct motor_pid_base<M2_VELOCITY_PID_CMD>;

}
}
}
