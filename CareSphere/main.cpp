#include <QCoreApplication>  // Use QCoreApplication instead of QApplication for CLI apps
#include <QTextStream>
#include "login.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);  // Use QCoreApplication instead of QApplication
    QTextStream qout(stdout);

    Login loginSystem;

    // Register users
    qout << "Registering users...\n";
    loginSystem.Register("JohnDoe", "secure123");
    loginSystem.Register("Alice", "mypassword");
    loginSystem.Register("Bob", "bobpass");

    // Testing valid login
    qout << "\nTesting valid logins:\n";
    qout << "JohnDoe login: " << (loginSystem.verify("JohnDoe", "secure123") ? "Success" : "Failure") << "\n";
    qout << "Alice login: " << (loginSystem.verify("Alice", "mypassword") ? "Success" : "Failure") << "\n";

    qout.flush();

    // Ensure program exits immediately
    QCoreApplication::quit();
    return 0;
}
