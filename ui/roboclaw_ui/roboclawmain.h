#ifndef ROBOCLAWMAIN_H
#define ROBOCLAWMAIN_H

#include <QMainWindow>
#include <QTimer>
#include "roboclaw/io/io.hpp"

namespace Ui
{
class RoboclawMain;
}

class RoboclawMain : public QMainWindow
{
    Q_OBJECT

  public:
    explicit RoboclawMain(QWidget* parent = 0);
    ~RoboclawMain();

  public slots:
    void updateSerialData();
    void stopAll();
    void changeM1Duty(int value);
    void changeM2Duty(int value);
    void changeM1Pid(int value);
    void changeM2Pid(int value);

  private:
    Ui::RoboclawMain* ui;
    roboclaw::io::serial_controller controller;
    void clearAllSliders();

    static constexpr uint32_t QPPS_PER_SECOND = 40000;
};

#endif  // ROBOCLAWMAIN_H
