#include <QApplication>
#include "roboclawmain.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    RoboclawMain w;
    w.show();

    return a.exec();
}
