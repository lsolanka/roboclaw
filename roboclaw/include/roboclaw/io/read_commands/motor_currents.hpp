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

struct motor_currents
{
    struct return_type
    {
        quantity<current> m1;
        quantity<current> m2;
    };
    static constexpr uint8_t CMD = 49;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.m1 = read_value<uint16_t>(port, crc) / 100.f * amperes;
        r.m2 = read_value<uint16_t>(port, crc) / 100.f * amperes;
        return r;
    }
};

}
}
}
