#include <roboclaw/io/read_commands/encoder_mode.hpp>

namespace roboclaw
{
namespace io
{
namespace read_commands
{
encoder_mode::return_type encoder_mode::read_response(boost::asio::serial_port& port,
                                                      crc_calculator_16& crc,
                                                      boost::log::record_ostream& strm)
{
    return_type r;
    auto enc1 = read_value<uint8_t>(port, crc, strm);
    r.enc1.quadrature = !(enc1 & 0x01);
    r.enc1.enable_rc_analog = enc1 & 0x80;

    auto enc2 = read_value<uint8_t>(port, crc, strm);
    r.enc2.quadrature = !(enc2 & 0x01);
    r.enc2.enable_rc_analog = enc2 & 0x80;

    return r;
}

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
