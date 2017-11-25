#pragma once

#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.hpp"
#include "../io.hpp"

namespace roboclaw
{
namespace io
{
namespace write_commands
{

struct settings_to_eeprom
{
    static constexpr uint8_t CMD = 94;
    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const
    {
        uint32_t magic_value = 0xE22EAB7A;
        write_value(magic_value, port, crc, strm);
    }
};

}
}
}

