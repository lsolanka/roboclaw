#pragma once
#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.h"
#include "../io.h"

namespace roboclaw
{
namespace io
{
namespace read_commands
{

template<uint8_t command_id>
struct motor_current_limit
{
    struct return_type
    {
        quantity<current> min;
        quantity<current> max;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;

        r.max = read_value<uint32_t>(port, crc) / 100.f * amperes;
        r.min = read_value<uint32_t>(port, crc) / 100.f * amperes;

        return r;
    }
};
using m1_current_limit = motor_current_limit<135>;
using m2_current_limit = motor_current_limit<136>;

}
}
}
