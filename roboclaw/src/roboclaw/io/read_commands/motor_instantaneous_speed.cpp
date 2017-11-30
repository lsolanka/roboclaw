#include <roboclaw/io/read_commands/motor_instantaneous_speed.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{

motor_instantaneous_speed::return_type
motor_instantaneous_speed::read_response(boost::asio::serial_port& port,
                                         crc_calculator_16& crc,
                                         boost::log::record_ostream& strm)
{
    return_type r;

    r.m1 = read_value<int32_t>(port, crc, strm);
    r.m2 = read_value<int32_t>(port, crc, strm);

    return r;
}

std::string get_string(const motor_instantaneous_speed::return_type& speed)
{
    std::stringstream ss;
    ss << "m1: " << speed.m1 << ", m2: " << speed.m2;
    return ss.str();
}

}
}
}
