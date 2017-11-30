#include "roboclawmain.h"
#include "ui_roboclawmain.h"

#include <roboclaw/io/read_commands.hpp>
#include <roboclaw/io/write_commands.hpp>
#include <roboclaw/io/types.hpp>

namespace read_commands = roboclaw::io::read_commands;
namespace write_commands = roboclaw::io::write_commands;

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, logger_t)
{
    namespace expr = boost::log::expressions;
    logger_t lg;
    boost::log::add_common_attributes();
    boost::log::add_file_log(
                boost::log::keywords::file_name = "log.txt",
                boost::log::keywords::format = (
                        expr::stream << expr::format_date_time<     boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                        << " [" << expr::attr<     boost::log::trivial::severity_level >("Severity") << "] "
                        << expr::smessage
            )
    );
    boost::log::add_console_log(
                std::cout,
                boost::log::keywords::format = (
                        expr::stream << expr::format_date_time<     boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                        << " [" << expr::attr<     boost::log::trivial::severity_level >("Severity") << "] "
                        << expr::smessage
            )
    );
    return lg;
}

RoboclawMain::RoboclawMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoboclawMain),
    controller("/dev/ttyACM0", 0x80)
{
    ui->setupUi(this);

    connect(ui->stopAllButton, SIGNAL(pressed()), this, SLOT(stopAll()));
    connect(ui->m1DutySlider, SIGNAL(valueChanged(int)), this, SLOT(changeM1Duty(int)));
    connect(ui->m2DutySlider, SIGNAL(valueChanged(int)), this, SLOT(changeM2Duty(int)));
    connect(ui->m1PidSlider, SIGNAL(valueChanged(int)), this, SLOT(changeM1Pid(int)));
    connect(ui->m2PidSlider, SIGNAL(valueChanged(int)), this, SLOT(changeM2Pid(int)));

    QTimer* controllerInfoTimer = new QTimer(this);
    QObject::connect(controllerInfoTimer, &QTimer::timeout,
                     this, &RoboclawMain::updateSerialData);
    controllerInfoTimer->start(500);

    controller.write(write_commands::m1_velocity_pid{1.f, 0.5f, 0.25f, QPPS_PER_SECOND});
    controller.write(write_commands::m2_velocity_pid{1.f, 0.5f, 0.25f, QPPS_PER_SECOND});
}

RoboclawMain::~RoboclawMain()
{
    delete ui;
}

void RoboclawMain::clearAllSliders()
{
    ui->m1DutySlider->setValue(0);
    ui->m2DutySlider->setValue(0);
    ui->m1PidSlider->setValue(0);
    ui->m2PidSlider->setValue(0);
}

void RoboclawMain::stopAll()
{
    controller.write(write_commands::m1_m2_drive_duty{0});
    clearAllSliders();
}

void RoboclawMain::changeM1Duty(int value)
{
    float duty_value = value / 100.f;
    controller.write(write_commands::m1_drive_duty{duty_value});
    ui->m1DutySliderValue->setText(QString::number(duty_value));
}

void RoboclawMain::changeM2Duty(int value)
{
    float duty_value = value / 100.f;
    controller.write(write_commands::m2_drive_duty{duty_value});
    ui->m2DutySliderValue->setText(QString::number(duty_value));
}

void RoboclawMain::changeM1Pid(int value)
{
    int32_t pid_value = int32_t(value / 100.f * QPPS_PER_SECOND);
    controller.write(write_commands::m1_drive_qpps{pid_value});
    ui->m1PidSliderValue->setText(QString::number(pid_value));
}

void RoboclawMain::changeM2Pid(int value)
{
    int32_t pid_value = int32_t(value / 100.f * QPPS_PER_SECOND);
    controller.write(write_commands::m2_drive_qpps{pid_value});
    ui->m2PidSliderValue->setText(QString::number(pid_value));
}

void RoboclawMain::updateSerialData()
{
    ui->firmwareVersionEdit->setText(QString::fromStdString(
                    controller.read<read_commands::firmware_version>()));

    // Board temperature
    ui->t1Edit->setText(QString::fromStdString(
                read_commands::get_string(
                    controller.read<read_commands::board_temperature_1>())));
    ui->t2Edit->setText(QString::fromStdString(
                read_commands::get_string(
                    controller.read<read_commands::board_temperature_2>())));

    // Battery voltage
    ui->mainBattVoltageEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(
                    controller.read<read_commands::main_battery_voltage>())));
    ui->logicBattVoltageEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(
                    controller.read<read_commands::logic_battery_voltage>())));

    // Encoders
    ui->m1EncEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(
                    controller.read<read_commands::m1_encoder_count>())));
    ui->m2EncEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(
                    controller.read<read_commands::m2_encoder_count>())));

    // Motor Amps
    auto motor_amps = controller.read<read_commands::motor_currents>();
    ui->m1AmpsEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(motor_amps.m1)));
    ui->m2AmpsEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(motor_amps.m2)));

    // Motor speed
    auto motor_speed = controller.read<read_commands::motor_instantaneous_speed>();
    ui->m1SpeedEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(motor_speed.m1)));
    ui->m2SpeedEdit->setText(QString::fromStdString(
                roboclaw::io::get_string(motor_speed.m2)));

}
