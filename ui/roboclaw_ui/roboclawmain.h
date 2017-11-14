#ifndef ROBOCLAWMAIN_H
#define ROBOCLAWMAIN_H

#include <QMainWindow>

namespace Ui {
class RoboclawMain;
}

class RoboclawMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoboclawMain(QWidget *parent = 0);
    ~RoboclawMain();

private:
    Ui::RoboclawMain *ui;
};

#endif // ROBOCLAWMAIN_H
