#include "roboclawmain.h"
#include "ui_roboclawmain.h"

RoboclawMain::RoboclawMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoboclawMain)
{
    ui->setupUi(this);
    setFixedSize(minimumSizeHint().width(), minimumSizeHint().height());
}

RoboclawMain::~RoboclawMain()
{
    delete ui;
}
