#include <QApplication>
#include <QSettings>
#include "login.h"
#include "prescription.h"
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

    Login loginWindow;
    loginWindow.show();

    int result = a.exec();

    QSettings settings("CareSphere", "Login System");
    settings.clear();
    settings.sync();

    return result;
}
