#include <QApplication>
#include "caresphere.h"
#include "login.h"
#include "registration.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Login w;
    w.show();

    return a.exec();
}
