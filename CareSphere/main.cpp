#include <QApplication>
#include "login.h"
#include "MySQL_utilities.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MySQLinit();

    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
