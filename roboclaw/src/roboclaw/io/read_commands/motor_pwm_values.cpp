#include <roboclaw/io/read_commands/motor_pwm_values.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{

motor_pwm_values::return_type
motor_pwm_values::read_response(boost::asio::serial_port& port, crc_calculator_16& crc, boost::log::record_ostream& strm)
{
    return_type r;
    r.m1 = read_value<uint16_t>(port, crc, strm) / 327.67f;
    r.m2 = read_value<uint16_t>(port, crc, strm) / 327.67f;
    return r;
}

std::string get_string(const motor_pwm_values::return_type& v)
{
    std::stringstream ss;
    ss << "m1: " << v.m1 << ", m2: " << v.m2;
    return ss.str();
}

}
}
}
