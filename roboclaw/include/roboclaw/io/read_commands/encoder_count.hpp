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
namespace read_commands
{
template<uint8_t command_id>
struct encoder_count_base
{
    using return_type = roboclaw::io::encoder_count;
    static constexpr uint8_t CMD = command_id;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     std::string& log_str);
};

static constexpr uint8_t M1_ENCODER_COUNT_CMD = 16;
static constexpr uint8_t M2_ENCODER_COUNT_CMD = 17;
using m1_encoder_count = encoder_count_base<M1_ENCODER_COUNT_CMD>;
using m2_encoder_count = encoder_count_base<M2_ENCODER_COUNT_CMD>;

extern template struct encoder_count_base<M1_ENCODER_COUNT_CMD>;
extern template struct encoder_count_base<M2_ENCODER_COUNT_CMD>;

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
