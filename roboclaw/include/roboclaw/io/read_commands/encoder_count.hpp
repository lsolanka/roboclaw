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
struct encoder_count_base
{
    struct return_type
    {
        uint16_t count;
        struct _status
        {
            uint8_t underflow : 1;
            uint8_t overflow : 1;
            uint8_t forward : 1;
        } status;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;

        r.count = read_value<uint32_t>(port, crc);

        auto status = read_value<uint8_t>(port, crc);
        r.status.underflow = status & 0x01;
        r.status.forward = !(status & 0x02);
        r.status.overflow = status & 0x04;

        return r;
    }

};
using m1_encoder_count = encoder_count_base<16>;
using m2_encoder_count = encoder_count_base<17>;

template<typename T>
std::string _get_encoder_count_string(const typename T::return_type& m)
{
    std::stringstream ss;
    ss << "count: " << m.count << " ["
       << (m.status.underflow ? "underflow," : "")
       << (m.status.overflow ? "overflow," : "")
       << (m.status.forward ? "forward" : "backward")
       << "]";

    return ss.str();
}
std::string get_string(const m1_encoder_count::return_type& m)
{
    return _get_encoder_count_string<m1_encoder_count>(m);
}
std::string get_string(const m2_encoder_count::return_type& m)
{
    return _get_encoder_count_string<m2_encoder_count>(m);
}

}
}
}
