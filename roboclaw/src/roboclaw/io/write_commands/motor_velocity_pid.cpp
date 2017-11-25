#include <roboclaw/io/write_commands/motor_velocity_pid.hpp>

namespace roboclaw
{
namespace io
{
namespace write_commands
{

template<uint8_t command_id>
void motor_pid_base<command_id>::write(boost::asio::serial_port& port, crc_calculator_16& crc,
           boost::log::record_ostream& strm) const
{
    write_value(uint32_t(d * 65536.f), port, crc, strm);
    write_value(uint32_t(p * 65536.f), port, crc, strm);
    write_value(uint32_t(i * 65536.f), port, crc, strm);
    write_value(qpps, port, crc, strm);
}

template struct motor_pid_base<M1_VELOCITY_PID_CMD>;
template struct motor_pid_base<M2_VELOCITY_PID_CMD>;
}
}
}
