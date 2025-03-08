#include <QApplication>
#include "caresphere.h"
#include "login.h"
#include "registration.h"
#include "user.h"
#include "utilities.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Login w;
    w.show();

    vector<User> users;

    // **Step 1: Create and Display Users**
    cout << "Creating users..." << endl;
    users.push_back(User("U001", "Alice Johnson", "alice@example.com", "1234567890", female, Christianity, "1995-06-10"));
    users.push_back(User("U002", "Bob Smith", "bob@example.com", "9876543210", male, Islam, "1992-04-15"));
    users.push_back(User("U003", "Charlie Adams", "charlie@example.com", "5551234567", male, Atheism, "1990-12-05"));

    // **Step 2: Save Users to CSV**
    cout << "\nSaving users to CSV..." << endl;
    saveUsers(users);

    // **Step 3: Load Users from CSV**
    cout << "\nLoading users from CSV..." << endl;
    vector<User> loadedUsers = loadUsers();

    // **Step 4: Display Loaded Users**
    cout << "\nDisplaying loaded users:\n";
    for (const auto& user : loadedUsers) {
        cout << "User ID: " << user.getId() << endl;
        cout << "Name: " << user.getName() << endl;
        cout << "Email: " << user.getEmail() << endl;
        cout << "Phone Number: " << user.getPhoneNo() << endl;
        cout << "Gender: " << (user.getGender() == male ? "Male" : "Female") << endl;
        cout << "Religion: " << user.getReligion() << endl;  // Enum will return integer
        cout << "Date of Birth: " << user.getDOB() << endl;
        cout << "---------------------\n";
    }

    // **Step 5: Run MySQL**
    connectAndRunQuery();



    return a.exec();
}
