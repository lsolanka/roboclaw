#pragma once

#include <boost/asio.hpp>
#include "../units.hpp"

#include "../crc_calculator.hpp"
#include "../io.hpp"
#include "../types.hpp"

namespace roboclaw
{
namespace io
{
namespace write_commands
{

template<uint8_t command_id>
struct encoder_mode_base : public roboclaw::io::encoder_mode
{
    static constexpr uint8_t CMD = command_id;

    encoder_mode_base() {}

    encoder_mode_base(uint8_t quadrature, uint8_t enable_rc_analog)
        : roboclaw::io::encoder_mode{quadrature, enable_rc_analog}
    {}

    bool write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const
    {
        uint8_t mode = (!quadrature) | (enable_rc_analog << 7);
        write_value(mode, port, crc, strm);
        return true;
    }
};
using m1_encoder_mode = encoder_mode_base<92>;
using m2_encoder_mode = encoder_mode_base<93>;

}
}
}
