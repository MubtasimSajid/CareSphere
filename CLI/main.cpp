#include "include/Database.h"
#include "include/User.h"
#include "include/Dashboard.h"
#include "include/Appointment.h"
#include "include/Medical.h"
#include "include/Billing.h"
#include "include/Notification.h"
#include <iostream>
#include <limits>

// Undefine max macro to avoid conflict with numeric_limits
#ifdef max
#undef max
#endif

using namespace std;

void displayMenu() {
    cout << "\n=================================\n";
    cout << "    CareSphere Medical System    \n";
    cout << "=================================\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "---------------------------------\n";
    cout << "Enter choice: ";
}

int main() {
    if (!Database::connect()) {
        cerr << "Failed to connect to database\n";
        return 1;
    }

    int choice;
    while (true) {
        displayMenu();
        
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        switch (choice) {
            case 1:
                User::registerUser();
                break;
            case 2:
                User::loginUser();
                break;
            case 3:
                cout << "Thank you for using CareSphere!\n";
                Database::disconnect();
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}