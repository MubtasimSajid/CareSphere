#include "caresphere.h"
#include <QDebug>
#include <QApplication>
#include <QTextStream>
<<<<<<< Updated upstream
#include <iostream>
#include "login.h"
=======
#include<iostream>
using namespace std;
>>>>>>> Stashed changes

using namespace std;



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CareSphere w;


    cout << "Hello from qout!";

    qInfo() << "Hello from qInfo()";     // Using qInfo() for comparison

    w.show();

    QTextStream qout(stdout);
    Login loginSystem;

    // Register users
    loginSystem.Register("JohnDoe", "secure123");
    loginSystem.Register("Alice", "mypassword");
    loginSystem.Register("Bob", "bobpass");

    // Testing valid login
    qout << "Testing valid logins:\n";
    qout << "JohnDoe login: " << (loginSystem.verify("JohnDoe", "secure123") ? "Success" : "Failure") << "\n";
    qout << "Alice login: " << (loginSystem.verify("Alice", "mypassword") ? "Success" : "Failure") << "\n";

    // Testing invalid login
    qout << "\nTesting invalid logins:\n";
    qout << "JohnDoe incorrect password: " << (loginSystem.verify("JohnDoe", "wrongpass") ? "Success" : "Failure") << "\n";
    qout << "Unknown user: " << (loginSystem.verify("Charlie", "somepass") ? "Success" : "Failure") << "\n";

    qout.flush();

    return a.exec();
}
