#include "caresphere.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CareSphere w;
    w.show();
    return a.exec();
}
