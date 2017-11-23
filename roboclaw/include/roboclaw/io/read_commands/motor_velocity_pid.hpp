#pragma once
#include <boost/asio.hpp>
#include "units.hpp"

#include "../crc_calculator.h"
#include "../io.h"

namespace roboclaw
{
namespace io
{
namespace read_commands
{

template<uint8_t command_id>
struct motor_pid_base
{
    struct return_type
    {
        float p;
        float i;
        float d;
        float qpps;
    };
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;

        r.p = read_value<uint32_t>(port, crc) / 65536.f;
        r.i = read_value<uint32_t>(port, crc) / 65536.f;
        r.d = read_value<uint32_t>(port, crc) / 65536.f;
        r.qpps = read_value<uint32_t>(port, crc);

        return r;
    }
};
using m1_velocity_pid = motor_pid_base<55>;
using m2_velocity_pid = motor_pid_base<56>;

template<uint8_t command_id>
std::string get_string(const typename motor_pid_base<command_id>::return_type& m)
{
    std::stringstream ss;
    ss << "p: " << m.p << ", i: " << m.i << ", d: " << m.d << ", qpps: " << m.qpps;
    return ss.str();
}
std::string get_string(const m1_velocity_pid::return_type& m)
{
    return get_string<m1_velocity_pid::CMD>(m);
}
std::string get_string(const m2_velocity_pid::return_type& m)
{
    return get_string<m2_velocity_pid::CMD>(m);
}

}
}
}
