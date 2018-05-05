#pragma once

#include <boost/asio.hpp>
#include "../units.hpp"

#include "../../logging.hpp"
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
struct drive_duty_base
{
    float duty_cycle;

    static constexpr uint8_t CMD = command_id;

    drive_duty_base(float duty);

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const;
};

using m1_drive_duty = drive_duty_base<32>;
using m2_drive_duty = drive_duty_base<33>;

extern template struct drive_duty_base<32>;
extern template struct drive_duty_base<33>;

struct m1_m2_drive_duty
{
    float m1_duty;
    float m2_duty;

    static constexpr uint8_t CMD = 34;

    m1_m2_drive_duty(float m1, float m2) { init_duty(m1, m2); }
    m1_m2_drive_duty(float m1_m2_duty) { init_duty(m1_m2_duty, m1_m2_duty); }

    void init_duty(float m1, float m2);

    void write(boost::asio::serial_port& port, crc_calculator_16& crc,
               boost::log::record_ostream& strm) const;
};

}  // namespace write_commands
}  // namespace io
}  // namespace roboclaw
