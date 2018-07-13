#include <QApplication>
#include "usbioboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    USBIOBoard w;
    w.show();

    return a.exec();
}
