#include "rechner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Rechner w;
    w.show();
    return a.exec();
}
