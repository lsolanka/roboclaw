#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/asio/serial_port.hpp>

#include "roboclaw/io.h"
#include "roboclaw/read_commands.h"

namespace po = boost::program_options;

void read_info(roboclaw::io::serial_controller& controller)
{
    namespace read_commands = roboclaw::io::read_commands;
    std::cout << "Firmware version: " << controller.read<read_commands::firmware_version>();

    auto encoder_mode = controller.read<read_commands::encoder_mode>();
    std::cout << "Encoder mode: " << +encoder_mode.enc1 << ", " << +encoder_mode.enc2 << std::endl;

    std::cout << "Main battery voltage: " << controller.read<read_commands::main_battery_voltage>() << std::endl;
    std::cout << "Logic battery voltage: " << controller.read<read_commands::logic_battery_voltage>() << std::endl;

    auto motor_pwm = controller.read<read_commands::motor_pwm_values>();
    std::cout << "Motor PWM values: " << motor_pwm.m1 << ", " << motor_pwm.m2 << std::endl;

    auto motor_currents = controller.read<read_commands::motor_currents>();
    std::cout << "Motor currents: " << motor_currents.m1 << ", " << motor_currents.m2 << std::endl;

    auto main_battery_v_settings = controller.read<read_commands::main_battery_voltage_settings>();
    std::cout << "Main battery voltage settings: " << main_battery_v_settings.min << ", " << main_battery_v_settings.max << std::endl;

    auto logic_battery_v_settings = controller.read<read_commands::logic_battery_voltage_settings>();
    std::cout << "Logic battery voltage settings: " << logic_battery_v_settings.min << ", " << logic_battery_v_settings.max << std::endl;

    auto temperature = controller.read<read_commands::temperature>();
    std::cout << "Temperature: " << temperature << std::endl;
}

int main(int argc, char** argv)
{
    std::string port_name;
    int speed;

    po::options_description desc("Allowed options:");
    desc.add_options()
        ("help", "Print help message")
        ("port,p", po::value<std::string>(&port_name)->default_value("/dev/ttyACM0"),
            "Serial port to use to connect to roboclaw")
        ("speed,s", po::value<int>(&speed), "Speed of the motors");


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc;
        return EXIT_SUCCESS;
    }
    //if (vm.count("speed") == 0) {
    //    std::cerr << "error: speed required" << std::endl;
    //    return EXIT_FAILURE;
    //}


    roboclaw::io::serial_controller controller(port_name, 0x80);
    read_info(controller);

    return EXIT_SUCCESS;
}
