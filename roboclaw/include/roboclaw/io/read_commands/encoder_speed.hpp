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
struct encoder_speed_values
{
    uint16_t speed;
    bool forward;
};

template<uint8_t command_id>
struct encoder_speed_base
{
    using return_type = encoder_speed_values;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str);
};

std::string get_string(const encoder_speed_values& m);

static constexpr uint8_t M1_ENCODER_SPEED_CMD = 18;
static constexpr uint8_t M2_ENCODER_SPEED_CMD = 19;
static constexpr uint8_t M1_RAW_SPEED_CMD = 30;
static constexpr uint8_t M2_RAW_SPEED_CMD = 31;

using m1_encoder_speed = encoder_speed_base<M1_ENCODER_SPEED_CMD>;
using m2_encoder_speed = encoder_speed_base<M2_ENCODER_SPEED_CMD>;
using m1_raw_speed = encoder_speed_base<M1_RAW_SPEED_CMD>;
using m2_raw_speed = encoder_speed_base<M2_RAW_SPEED_CMD>;

extern template struct encoder_speed_base<M1_ENCODER_SPEED_CMD>;
extern template struct encoder_speed_base<M2_ENCODER_SPEED_CMD>;
extern template struct encoder_speed_base<M1_RAW_SPEED_CMD>;
extern template struct encoder_speed_base<M2_RAW_SPEED_CMD>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
