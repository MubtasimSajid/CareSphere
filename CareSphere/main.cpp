#include <QApplication>
#include<vector>
#include "login.h"
#include "user.h"
#include "MySQL_utilities.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
