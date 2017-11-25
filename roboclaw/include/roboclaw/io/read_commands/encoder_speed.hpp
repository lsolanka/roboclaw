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
struct encoder_speed_base
{
    struct return_type
    {
        uint16_t speed;
        bool forward;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc, boost::log::record_ostream& strm)
    {
        return_type r;

        r.speed = read_value<uint32_t>(port, crc, strm);
        r.forward = !(read_value<uint8_t>(port, crc, strm) & 0x01);

        return r;
    }

};
using m1_encoder_speed = encoder_speed_base<18>;
using m2_encoder_speed = encoder_speed_base<19>;
using m1_raw_speed = encoder_speed_base<30>;
using m2_raw_speed = encoder_speed_base<31>;

template<typename T>
std::string _get_encoder_speed_string(const typename T::return_type& m)
{
    std::stringstream ss;
    ss << "speed: " << m.speed << " " << (m.forward ? "[forward]" : "[backward]");
    return ss.str();
}
std::string get_string(const m1_encoder_speed::return_type& m)
{
    return _get_encoder_speed_string<m1_encoder_speed>(m);
}
std::string get_string(const m2_encoder_speed::return_type& m)
{
    return _get_encoder_speed_string<m2_encoder_speed>(m);
}
std::string get_string(const m1_raw_speed::return_type& m)
{
    return _get_encoder_speed_string<m1_raw_speed>(m);
}
std::string get_string(const m2_raw_speed::return_type& m)
{
    return _get_encoder_speed_string<m2_raw_speed>(m);
}

}
}
}
