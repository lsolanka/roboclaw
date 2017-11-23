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
struct battery_voltage_settings_base
{
    struct return_type
    {
        quantity<electric_potential> min;
        quantity<electric_potential> max;
    };
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.min = read_value<uint16_t>(port, crc) / 10.f * volts;
        r.max = read_value<uint16_t>(port, crc) / 10.f * volts;
        return r;
    }
};
using main_battery_voltage_settings = battery_voltage_settings_base<59>;
using logic_battery_voltage_settings = battery_voltage_settings_base<60>;

}
}
}
