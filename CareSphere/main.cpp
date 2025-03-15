#include <QApplication>
#include "login.h"
#include"prescription.h"
#include "MySQL_utilities.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MySQLinit();

    Prescription test("F-00", "WAZED", "ACE-PLUS, PERACETAMOL, NAPA");
    test.displayDetails();
    // SavePrescription(test);
    string check = GetUserPrescriptions("F-00");
    cout<<check<<endl;


    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
