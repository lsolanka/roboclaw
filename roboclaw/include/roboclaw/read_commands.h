#pragma once

#include <string>
#include <boost/asio.hpp>

#include "io.h"
#include "crc_calculator.h"

namespace roboclaw
{
namespace io
{

namespace read_commands
{

struct firmware_version
{
    using return_type = std::string;
    static constexpr uint8_t CMD = 21;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
       char c;
       std::string result;
       while (true) {
           boost::asio::read(port, boost::asio::buffer(&c, 1));
           if (c != 0) {
               result += c;
           }
           else {
               break;
           }
       }

       crc << result;
       return result;
    }
};

template<uint8_t command_id>
struct battery_voltage_base
{
    using return_type = float;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc)
    {
        uint16_t value = read_value<uint16_t>(port, crc);
        return value / 10.;
    }
};
using main_battery_voltage = battery_voltage_base<24>;
using logic_battery_voltage = battery_voltage_base<25>;

struct motor_pwm_values
{
    struct return_type
    {
        float m1;
        float m2;
    };
    static constexpr uint8_t CMD = 48;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.m1 = read_value<uint16_t>(port, crc) / 327.67f;
        r.m2 = read_value<uint16_t>(port, crc) / 327.67f;
        return r;
    }
};

struct motor_currents
{
    struct return_type
    {
        float m1;
        float m2;
    };
    static constexpr uint8_t CMD = 49;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.m1 = read_value<uint16_t>(port, crc) / 100.f;
        r.m2 = read_value<uint16_t>(port, crc) / 100.f;
        return r;
    }
};

template<uint8_t command_id>
struct battery_voltage_settings_base
{
    struct return_type
    {
        float min;
        float max;
    };
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.min = read_value<uint16_t>(port, crc) / 10.f;
        r.max = read_value<uint16_t>(port, crc) / 10.f;
        return r;
    }
};
using main_battery_voltage_settings = battery_voltage_settings_base<59>;
using logic_battery_voltage_settings = battery_voltage_settings_base<60>;

template<uint8_t command_id>
struct temperature_base
{
    using return_type = float;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return read_value<uint16_t>(port, crc) / 10.f;
    }
};
using board_temperature_1 = temperature_base<82>;
using board_temperature_2 = temperature_base<83>;

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
    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        auto value = read_value<uint16_t>(port, crc);
        return_type r;

        r.m1_overcurrent            = value & 0x0001;
        r.m2_overcurrent            = value & 0x0002;
        r.e_stop                    = value & 0x0004;
        r.temperature_error         = value & 0x0008;
        r.temperature2_error        = value & 0x0010;
        r.main_battery_high_error   = value & 0x0020;
        r.logic_battery_high_error  = value & 0x0040;
        r.logic_battery_low_error   = value & 0x0080;
        r.m1_driver_fault           = value & 0x0100;
        r.m2_driver_fault           = value & 0x0200;
        r.main_battery_high_warning = value & 0x0400;
        r.main_battery_low_warning  = value & 0x0800;
        r.temperature_warning       = value & 0x1000;
        r.temperature2_warning      = value & 0x2000;
        r.m1_home                   = value & 0x4000;
        r.m2_home                   = value & 0x8000;

        return r;
    }
};

std::string get_string(const status::return_type& s)
{
    std::stringstream ss;
    ss << "  M1 OverCurrent Warning    " << s.m1_overcurrent            << '\n'
       << "  M2 OverCurrent Warning    " << s.m2_overcurrent            << '\n'
       << "  E-Stop                    " << s.e_stop                    << '\n'
       << "  Temperature Error         " << s.temperature_error         << '\n'
       << "  Temperature2 Error        " << s.temperature2_error        << '\n'
       << "  Main Battery High Error   " << s.main_battery_high_error   << '\n'
       << "  Logic Battery High Error  " << s.logic_battery_high_error  << '\n'
       << "  Logic Battery Low Error   " << s.logic_battery_low_error   << '\n'
       << "  M1 Driver Fault           " << s.m1_driver_fault           << '\n'
       << "  M2 Driver Fault           " << s.m2_driver_fault           << '\n'
       << "  Main Battery High Warning " << s.main_battery_high_warning << '\n'
       << "  Main Battery Low Warning  " << s.main_battery_low_warning  << '\n'
       << "  Termperature Warning      " << s.temperature_warning       << '\n'
       << "  Temperature2 Warning      " << s.temperature2_warning      << '\n'
       << "  M1 Home                   " << s.m1_home                   << '\n'
       << "  M2 Home                   " << s.m2_home                   << '\n';

    return ss.str();
}

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
        r.enc1.quadrature = !(enc2 & 0x01);
        r.enc2.enable_rc_analog = enc2 & 0x80;

        return r;
    }
};

std::string get_string(const encoder_mode::return_type& m)
{
    auto print_one = [] (const std::string& heading, const encoder_mode::return_type::mode& m)
    {
        std::stringstream ss;
        ss << heading << (m.quadrature ? "quadrature" : "analog")
                      << (m.enable_rc_analog ? ", rc/analog enabled" : "");
        return ss.str(); 
    };

    std::string result = "";
    result += print_one("encoder1: ", m.enc1) + "; " + print_one("encoder2: ", m.enc2);

    return result;
}

template<uint8_t command_id>
struct motor_current_limit
{
    struct return_type
    {
        float min;
        float max;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;

        r.max = read_value<uint32_t>(port, crc) / 100.f;
        r.min = read_value<uint32_t>(port, crc) / 100.f;

        return r;
    }
};
using m1_current_limit = motor_current_limit<135>;
using m2_current_limit = motor_current_limit<136>;

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


template<uint8_t command_id>
struct encoder_speed_base
{
    struct return_type
    {
        uint16_t speed;
        bool forward;
    };

    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;

        r.speed = read_value<uint32_t>(port, crc);
        r.forward = !(read_value<uint8_t>(port, crc) & 0x01);

        return r;
    }

};
using m1_encoder_speed = encoder_speed_base<18>;
using m2_encoder_speed = encoder_speed_base<19>;

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

} // namespace read_commands
} // namespace io
} // namespace roboclaw
