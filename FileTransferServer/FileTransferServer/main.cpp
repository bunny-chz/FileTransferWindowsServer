#include <QApplication>
#include "pcserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PcServer w;
    w.show();
    return a.exec();
}


