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
struct temperature_base
{
    using return_type = quantity<temperature>;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return read_value<uint16_t>(port, crc) / 10.f * celsius::degree;
    }
};
using board_temperature_1 = temperature_base<82>;
using board_temperature_2 = temperature_base<83>;

}
}
}
