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

    // // Testing Prescriptions
    // Prescription test("F-00", "WAZED", "ACE-PLUS, PERACETAMOL, NAPA");
    // // // test.displayDetails();
    // // SavePrescription(test);
    // DeletePrescription("F-00", "WAZED", "ACE-PLUS, PERACETAMOL, NAPA");
    // // UpdatePrescription("F-00", "FTK", "No Medicines", "WAZED", "ACE-PLUS, PERACETAMOL, NAPA");
    // vector<string> check = GetUserPrescriptions("F-00");
    // for(auto x: check){
    //     qInfo()<<x;
    // }


    // Testing Notes
    // string notes = "Eat Apple";
    // save_User_Notes("new3", "banana");
    // vector<string> Notes = get_User_Notes("new3");
    // for (auto x: Notes){
    //     cout<<x<<endl;
    // }
    // Delete_User_Notes("new3", "banana");
    // string check2 = get_User_Notes("new3");
    // cout<<check2<<endl<<endl;

    // // Testing Reminders
    // // Creating a reminder object
    // Reminder r1("F-00", "Doctor Appointment", "2025-12-01", "12:30:00", "Visit Dr. Smith for a checkup");
    // Save_User_Reminder(r1);
    // Update_Reminder("F-00", "Test", "2025-30-01", "13:00", "Test Is running","Doctor Appointment","2025-12-01","12:30:00", "Visit Dr. Smith for a checkup"   );
    // Delete_Reminder("F-00", "Test", "2025-30-01", "13:00", "Test Is running");

    // // Testing Appointments
    // // Creating an appointment object
    // // Appointment a1("F-00", "Test", "Mirpur", "01-12-2025", "9:00 PM");
    // // Save_User_Appointment(a1);
    // string check4 = Get_User_Appointments("F-00");
    // cout<<check4<<endl;
    // // Displaying the formatted output
    // // cout << a1.formatAppointment() << endl;

    Login loginWindow;
    loginWindow.show();

    return a.exec();
}
