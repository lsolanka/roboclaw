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
struct drive_qpps_base
{
    int32_t qpps;

    static constexpr uint8_t CMD = command_id;

    drive_qpps_base(int32_t value) : qpps(value) { }

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const
    {
        write_value(qpps, port, crc, strm);
    }
};

using m1_drive_qpps = drive_qpps_base<35>;
using m2_drive_qpps = drive_qpps_base<36>;

struct m1_m2_drive_qpps
{
    int32_t m1_qpps;
    int32_t m2_qpps;

    static constexpr uint8_t CMD = 37;

    m1_m2_drive_qpps(int32_t m1, int32_t m2) :
        m1_qpps(m1), m2_qpps(m2) { }

    m1_m2_drive_qpps(int32_t m1_m2) :
        m1_qpps(m1_m2), m2_qpps(m1_m2) {  }

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const
    {
        write_value(m1_qpps, port, crc, strm);
        write_value(m2_qpps, port, crc, strm);
    }
};

}
}
}


