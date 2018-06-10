#include <roboclaw/io/write_commands/drive_duty.hpp>
#include <roboclaw/logging.hpp>

namespace roboclaw
{
namespace io
{
namespace write_commands
{
template<uint8_t command_id>
drive_duty_base<command_id>::drive_duty_base(float duty) : duty_cycle(duty)
{
    if (duty_cycle < -1.f || duty_cycle > 1.f)
    {
        auto logger = get_roboclaw_logger();
        logger->error(
                "command: {:d}: invalid duty cycle value: {:.5g}. Duty cycle will be set "
                "to range [-1, 1].",
                uint8_t(CMD), duty_cycle);
    }
    duty_cycle = std::max(std::min(duty_cycle, 1.f), -1.f);
}

template<uint8_t command_id>
void drive_duty_base<command_id>::write(boost::asio::serial_port& port,
                                        crc_calculator_16& crc,
                                        std::string& log_str) const
{
    int32_t value = duty_cycle * 32767.f;
    value = std::max(std::min(value, 32767), -32767);
    write_value(int16_t(value), port, crc, log_str);
}

template struct drive_duty_base<32>;
template struct drive_duty_base<33>;

/* ------------------------------------------------------------------------- */

void m1_m2_drive_duty::init_duty(float m1, float m2)
{
    if (m1 < -1.f || m1 > 1.f)
    {
        auto logger = get_roboclaw_logger();
        logger->error(
                "command: {:d}: invalid M1 duty cycle value: {:.5g}. Duty cycle will be "
                "set to range [-1, 1].",
                uint8_t(CMD), m1);
    }
    if (m2 < -1.f || m2 > 1.f)
    {
        auto logger = get_roboclaw_logger();
        logger->error(
                "command: {:d}: invalid M2 duty cycle value: {:.5g}. Duty cycle will be "
                "set to range [-1, 1].",
                uint8_t(CMD), m1);
    }

    m1_duty = std::max(std::min(m1, 1.f), -1.f);
    m2_duty = std::max(std::min(m2, 1.f), -1.f);
}

void m1_m2_drive_duty::write(boost::asio::serial_port& port, crc_calculator_16& crc,
                             std::string& log_str) const
{
    int32_t m1_value = m1_duty * 32767.f;
    m1_value = std::max(std::min(m1_value, 32767), -32767);
    write_value(int16_t(m1_value), port, crc, log_str);

    int32_t m2_value = m2_duty * 32767.f;
    m2_value = std::max(std::min(m2_value, 32767), -32767);
    write_value(int16_t(m2_value), port, crc, log_str);
}

}  // namespace write_commands
}  // namespace io
}  // namespace roboclaw
