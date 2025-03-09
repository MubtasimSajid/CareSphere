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

    // Testing the User Class using the MySQL DB. SUCCESSFUL .......
    vector<User> users;
    MySQLinit();
    User test("01", "Farhan", "ayon55928@gmail.com", "01735429709");
    saveUser(test);
    users = loadUsers();
    // Printing the vector
    for(auto user: users){
        user.details();
    }

    return a.exec();
}
