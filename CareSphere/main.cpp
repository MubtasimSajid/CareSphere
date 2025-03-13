#include <QApplication>
#include<vector>
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

    // Testing the User Class using the MySQL DB. SUCCESSFUL .......
    vector<User> users;
    MySQLinit();
    User test("01", "Farhan", "ayon55928@gmail.com", "01735429709", stringToGender("male") , stringToReligion("Islam"), "01-12-2003");
    saveUser(test);
    users = loadUsers();
    // Testing the GetUser Function... Remove if not needed
    User current_user = getUser(users, "Farhan");
    qInfo()<<"Printing the details of the User Found using the function";
    qInfo()<<"Name of the User: "<<current_user.getName();
    qInfo() << "Email of the User: " << QString::fromStdString(current_user.getEmail());
    qInfo() << "Phone Number of the User: " << QString::fromStdString(current_user.getPhoneNo());
    qInfo() << "Gender of the User: " << genderToString(current_user.getGender());
    qInfo() << "Religion of the User: " << religionToString( current_user.getReligion());
    qInfo() << "Date of Birth of the User: " << QString::fromStdString(current_user.getDOB());





    return a.exec();
}
