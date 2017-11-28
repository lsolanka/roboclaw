#ifndef ROBOCLAWMAIN_H
#define ROBOCLAWMAIN_H

#include <QMainWindow>
#include <QTimer>
#include "roboclaw/io/io.hpp"

namespace Ui {
class RoboclawMain;
}

class RoboclawMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoboclawMain(QWidget *parent = 0);
    ~RoboclawMain();

public slots:
    void updateSerialData();

private:
    Ui::RoboclawMain *ui;
    roboclaw::io::serial_controller controller;
};

#endif // ROBOCLAWMAIN_H
