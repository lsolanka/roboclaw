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

struct status
{
    struct return_type
    {
        unsigned m1_overcurrent : 1;
        unsigned m2_overcurrent : 1;
        unsigned e_stop : 1;
        unsigned temperature_error : 1;
        unsigned temperature2_error : 1;
        unsigned main_battery_high_error: 1;
        unsigned logic_battery_high_error: 1;
        unsigned logic_battery_low_error: 1;
        unsigned m1_driver_fault: 1;
        unsigned m2_driver_fault: 1;
        unsigned main_battery_high_warning: 1;
        unsigned main_battery_low_warning: 1;
        unsigned temperature_warning: 1;
        unsigned temperature2_warning: 1;
        unsigned m1_home: 1;
        unsigned m2_home: 1;
    };
    static constexpr uint8_t CMD = 90;
    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     boost::log::record_ostream& strm);
};

std::string get_string(const status::return_type& s);

}
}
}
