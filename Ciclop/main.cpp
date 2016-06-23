#include "Ciclop.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ciclop w;
    w.show();

    return a.exec();
}
