#include <roboclaw/io/read_commands/encoder_count.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{
template<uint8_t command_id>
typename encoder_count_base<command_id>::return_type
encoder_count_base<command_id>::read_response(boost::asio::serial_port& port,
                                              crc_calculator_16& crc,
                                              boost::log::record_ostream& strm)
{
    return_type r;

    r.count = read_value<uint32_t>(port, crc, strm);

    auto status = read_value<uint8_t>(port, crc, strm);
    r.status.underflow = status & 0x01;
    r.status.forward = !(status & 0x02);
    r.status.overflow = status & 0x04;

    return r;
}

template struct encoder_count_base<M1_ENCODER_COUNT_CMD>;
template struct encoder_count_base<M2_ENCODER_COUNT_CMD>;
}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
