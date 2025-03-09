#include <QApplication>
#include "caresphere.h"
#include "login.h"
#include "registration.h"
#include "user.h"
#include "utilities.h"
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

    vector<User> users;

    MySQLinit();
    QSqlQuery query;

    return a.exec();
}
