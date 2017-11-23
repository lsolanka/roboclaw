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

struct encoder_mode
{
    struct return_type
    {
        struct mode
        {
            unsigned quadrature : 1;
            unsigned enable_rc_analog : 1;
        };
        mode enc1;
        mode enc2;
    };
    static constexpr uint8_t CMD = 91;
    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        auto enc1 = read_value<uint8_t>(port, crc);
        r.enc1.quadrature = !(enc1 & 0x01);
        r.enc1.enable_rc_analog = enc1 & 0x80;

        auto enc2 = read_value<uint8_t>(port, crc);
        r.enc2.quadrature = !(enc2 & 0x01);
        r.enc2.enable_rc_analog = enc2 & 0x80;

        return r;
    }
};

std::string get_string(const encoder_mode::return_type& m)
{
    auto print_one = [] (const std::string& heading, const encoder_mode::return_type::mode& m)
    {
        std::stringstream ss;
        ss << heading << (m.quadrature ? "quadrature" : "absolute")
                      << (m.enable_rc_analog ? ", rc/analog enabled" : "");
        return ss.str(); 
    };

    std::string result = "";
    result += print_one("encoder1: ", m.enc1) + "; " + print_one("encoder2: ", m.enc2);

    return result;
}

}
}
}
