#include <roboclaw/io/read_commands/firmware_version.hpp>

std::string roboclaw::io::read_commands::firmware_version::read_response(
        boost::asio::serial_port& port, crc_calculator_16& crc,
        boost::log::record_ostream& strm)
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
   strm << result;
   return result;
}
