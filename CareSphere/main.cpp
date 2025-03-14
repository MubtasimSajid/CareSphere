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

    // Testing the User Functions
    vector<User> users;
    MySQLinit();
    // User test("Farhan-01", "Farhan", "ayon55928@gmail.com", "01735429709", "B+", "Male", "01-12-2001");
    // test.details();
    // saveUser(test);
    // users = loadUsers();
    // for(auto x: users){
    //     x.details();
    // }
    // users = loadUsers();
    // User current_user = getUser(users, "Farhan-01");
    // qInfo()<<"Printing the details of the User Found using the function";
    // qInfo()<<"Name of the User: "<<current_user.getName();
    // qInfo() << "Email of the User: " << QString::fromStdString(current_user.getEmail());
    // qInfo() << "Phone Number of the User: " << QString::fromStdString(current_user.getPhoneNo());
    // qInfo() << "Gender of the User: " <<  QString::fromStdString(current_user.getGender());
    // qInfo() << "Date of Birth of the User: " << QString::fromStdString(current_user.getDOB());

    return a.exec();
}
