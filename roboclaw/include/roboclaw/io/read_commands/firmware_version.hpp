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
struct firmware_version
{
    using return_type = std::string;
    static constexpr uint8_t CMD = 21;

    static return_type read_response(boost::asio::serial_port& port,
                                     crc_calculator_16& crc,
                                     boost::log::record_ostream& strm);
};

}  // namespace read_commands
}  // namespace io
}  // namespace roboclaw
