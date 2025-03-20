#include <QApplication>
#include "login.h"
#include"prescription.h"
#include "MySQL_utilities.h"
#include "reminders.h"
#include "appointment.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MySQLinit();
    QTextStream out(stdout);

    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
