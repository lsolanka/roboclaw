#pragma once

#include <boost/asio.hpp>
#include "../units.hpp"

#include "../types.hpp"
#include "../crc_calculator.hpp"
#include "../io.hpp"
#include "../../logging.hpp"

namespace roboclaw
{
namespace io
{
namespace write_commands
{

template<uint8_t command_id>
struct drive_duty_base
{
    float duty_cycle;

    static constexpr uint8_t CMD = command_id;

    drive_duty_base(float duty) : duty_cycle(duty)
    {
        if (duty_cycle < -1.f || duty_cycle > 1.f)
        {
            BOOST_LOG_TRIVIAL(error) << "command: " << +uint8_t(CMD) <<
                ": invalid duty cycle value: " << duty_cycle
                << ". Duty cycle will be set to range [-1, 1].";
        }
        duty_cycle = std::max(std::min(duty_cycle, 1.f), -1.f);
    }

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const
    {

        int32_t value = duty_cycle * 32767.f;
        value = std::max(std::min(value, 32767), -32767);
        write_value(int16_t(value), port, crc, strm);
    }
};

using m1_drive_duty = drive_duty_base<32>;
using m2_drive_duty = drive_duty_base<33>;
}
}
}

