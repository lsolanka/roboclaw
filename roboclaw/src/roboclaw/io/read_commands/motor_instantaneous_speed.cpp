#include <roboclaw/io/read_commands/motor_instantaneous_speed.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{
motor_instantaneous_speed::return_type motor_instantaneous_speed::read_response(
        boost::asio::serial_port& port, crc_calculator_16& crc,
        std::string& log_str)
{
    return_type r;

    r.m1 = read_value<int32_t>(port, crc, log_str);
    r.m2 = read_value<int32_t>(port, crc, log_str);

    return r;
}

std::string get_string(const motor_instantaneous_speed::return_type& speed)
{
    std::stringstream ss;
    ss << "m1: " << speed.m1 << ", m2: " << speed.m2;
    return ss.str();
}

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
