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

struct encoder_mode
{
    struct return_type
    {
        uint8_t enc1;
        uint8_t enc2;
    };
    static constexpr uint8_t CMD = 91;

    static return_type read_response(boost::asio::serial_port& port, crc_calculator_16& crc)
    {
        return_type r;
        r.enc1 = read_value<decltype(r.enc1)>(port, crc);
        r.enc2 = read_value<decltype(r.enc2)>(port, crc);
        return r;
    }
};

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

} // namespace read_commands
} // namespace io
} // namespace roboclaw
