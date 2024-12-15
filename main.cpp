#include "zavodMessenger.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZavodMessenger w;
    w.show();
    return a.exec();
}
