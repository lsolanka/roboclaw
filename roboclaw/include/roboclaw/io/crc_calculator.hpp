#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace roboclaw
{
namespace io
{
class crc_calculator_16
{
  public:
    crc_calculator_16() : crc(0) {}

    template<typename T>
    crc_calculator_16& operator<<(const T& data)
    {
        static_assert(std::is_pod<T>::value, "cannot calculate CRC from non-POD type");
        return add_buffer((const unsigned char*)&data, sizeof(T));
    }

    /** Adds the characters of s.c_str() to the crc, including the null
     * termination character.
     */
    crc_calculator_16& operator<<(const std::string& s)
    {
        return add_buffer((const unsigned char*)s.c_str(), s.size() + 1);
    }

    crc_calculator_16& add_buffer(const unsigned char* buffer, int size)
    {
        for (unsigned char byte = 0; byte < size; byte++)
        {
            crc = crc ^ ((uint16_t)buffer[byte] << 8);
            for (unsigned char bit = 0; bit < 8; bit++)
            {
                if (crc & 0x8000)
                {
                    crc = (crc << 1) ^ 0x1021;
                }
                else
                {
                    crc = crc << 1;
                }
            }
        }

        return *this;
    }

    uint16_t get() const { return crc; }

  private:
    uint16_t crc;
};

}  // namespace io
}  // namespace roboclaw
