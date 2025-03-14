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

    // // Testing the Prescription Class
    // // Create an object using parameterized constructor
    // Prescription p1("F-00", "PARACETAMOL", "500mg", "2 Times after Eating");
    // Prescription p2("F-00", "ACE PLUS", "200mg", "3 Times after Eating");

    // SavePrescription(p1);
    // SavePrescription(p2);

    // // Testing the Get Prescription function to get PRescription from the DB.
    // string test = GetUserPrescriptions("F-00");
    // cout<<test<<endl;

    string notes = "1. Avoid spending too much time on mobile devices\n"
                   "2. Drink 3L water every day\n"
                   "3. Eat Fruits to replenish the Vitamin deficiencies";

    // qInfo()<<notes;
    // save_User_Notes("F-00", notes);
    string test = get_User_Notes("F-00");
    cout<<test<<endl;


    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
