#include <roboclaw/io/read_commands/motor_velocity_pid.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{
template<uint8_t command_id>
roboclaw::io::pid_values motor_pid_base<command_id>::read_response(
        boost::asio::serial_port& port, crc_calculator_16& crc,
        std::string& log_str)
{
    return_type r;

    r.p = read_value<uint32_t>(port, crc, log_str) / 65536.f;
    r.i = read_value<uint32_t>(port, crc, log_str) / 65536.f;
    r.d = read_value<uint32_t>(port, crc, log_str) / 65536.f;
    r.qpps = read_value<uint32_t>(port, crc, log_str);

    return r;
}

template struct motor_pid_base<55>;
template struct motor_pid_base<56>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
