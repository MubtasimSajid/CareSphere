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

    // // Display prescription details
    // qInfo() << "User: " << p1.getUserName();
    // qInfo() << "Medicine: " << p1.getMedicine();
    // qInfo() << "Dosage: " << p1.getDosage();
    // qInfo() << "Frequency: " << p1.getFrequency();


    // // Display updated details
    // qInfo() << "\nUpdated Prescription:";
    // qInfo() << "Dosage: " << p1.getDosage();
    // qInfo() << "Frequency: " << p1.getFrequency();

    // SavePrescription(p1);

    // Testing the Get Prescription function to get PRescription from the DB.
    string test = GetUserPrescriptions("F-00");
    cout<<test<<endl;
    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
