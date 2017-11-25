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

struct motor_instantaneous_speed
{
    struct return_type
    {
        uint32_t m1;
        uint32_t m2;
    };
    static constexpr uint8_t CMD = 79;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc, boost::log::record_ostream& strm)
    {
        return_type r;

        r.m1 = read_value<uint32_t>(port, crc, strm);
        r.m2 = read_value<uint32_t>(port, crc, strm);

        return r;
    }
};

std::string get_string(const motor_instantaneous_speed::return_type& speed)
{
    std::stringstream ss;
    ss << "m1: " << speed.m1 << ", m2: " << speed.m2;
    return ss.str();
}

}
}
}
