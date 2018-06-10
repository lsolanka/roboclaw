#pragma once
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
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
struct battery_voltage_base
{
    using return_type = quantity<electric_potential>;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str)
    {
        uint16_t value = read_value<uint16_t>(port, crc, log_str);
        return (value / 10.f) * volts;
    }
};
using main_battery_voltage = battery_voltage_base<24>;
using logic_battery_voltage = battery_voltage_base<25>;

}  // namespace read_commands

}  // namespace io
}  // namespace roboclaw
