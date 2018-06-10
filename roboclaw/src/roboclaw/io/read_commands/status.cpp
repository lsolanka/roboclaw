#include <roboclaw/io/read_commands/status.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{
status::return_type status::read_response(boost::asio::serial_port& port,
                                          crc_calculator_16& crc,
                                          std::string& log_str)
{
    auto value = read_value<uint16_t>(port, crc, log_str);
    return_type r;

    r.m1_overcurrent = value & 0x0001;
    r.m2_overcurrent = value & 0x0002;
    r.e_stop = value & 0x0004;
    r.temperature_error = value & 0x0008;
    r.temperature2_error = value & 0x0010;
    r.main_battery_high_error = value & 0x0020;
    r.logic_battery_high_error = value & 0x0040;
    r.logic_battery_low_error = value & 0x0080;
    r.m1_driver_fault = value & 0x0100;
    r.m2_driver_fault = value & 0x0200;
    r.main_battery_high_warning = value & 0x0400;
    r.main_battery_low_warning = value & 0x0800;
    r.temperature_warning = value & 0x1000;
    r.temperature2_warning = value & 0x2000;
    r.m1_home = value & 0x4000;
    r.m2_home = value & 0x8000;

    return r;
}

std::string get_string(const status::return_type& s)
{
    std::stringstream ss;
    ss << "  M1 OverCurrent Warning    " << s.m1_overcurrent << '\n'
       << "  M2 OverCurrent Warning    " << s.m2_overcurrent << '\n'
       << "  E-Stop                    " << s.e_stop << '\n'
       << "  Temperature Error         " << s.temperature_error << '\n'
       << "  Temperature2 Error        " << s.temperature2_error << '\n'
       << "  Main Battery High Error   " << s.main_battery_high_error << '\n'
       << "  Logic Battery High Error  " << s.logic_battery_high_error << '\n'
       << "  Logic Battery Low Error   " << s.logic_battery_low_error << '\n'
       << "  M1 Driver Fault           " << s.m1_driver_fault << '\n'
       << "  M2 Driver Fault           " << s.m2_driver_fault << '\n'
       << "  Main Battery High Warning " << s.main_battery_high_warning << '\n'
       << "  Main Battery Low Warning  " << s.main_battery_low_warning << '\n'
       << "  Termperature Warning      " << s.temperature_warning << '\n'
       << "  Temperature2 Warning      " << s.temperature2_warning << '\n'
       << "  M1 Home                   " << s.m1_home << '\n'
       << "  M2 Home                   " << s.m2_home << '\n';

    return ss.str();
}

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
