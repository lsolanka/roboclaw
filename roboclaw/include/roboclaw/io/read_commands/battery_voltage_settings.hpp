#pragma once
#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.hpp"
#include "../io.hpp"

namespace roboclaw
{
namespace io
{
namespace read_commands
{
template<uint8_t command_id>
struct battery_voltage_settings_base
{
    struct return_type
    {
        units::quantity<units::electric_potential> min;
        units::quantity<units::electric_potential> max;
    };
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str)
    {
        return_type r;
        r.min = read_value<uint16_t>(port, crc, log_str) / 10.f * units::volts;
        r.max = read_value<uint16_t>(port, crc, log_str) / 10.f * units::volts;
        return r;
    }
};
using main_battery_voltage_settings = battery_voltage_settings_base<59>;
using logic_battery_voltage_settings = battery_voltage_settings_base<60>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
