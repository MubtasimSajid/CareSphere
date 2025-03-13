#include "../include/User.h"
#include "../include/Database.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    try {
        // Get database connection
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cerr << "Failed to connect to database" << endl;
            return 1;
        }

        User user;
        int choice;
        const int MAX_INPUT = 1000; // Define a reasonable max input size

        while (true) {
            cout << "\n=== CareSphere Health Management System ===\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "Enter your choice (1-3): ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(MAX_INPUT, '\n');
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            cin.ignore(MAX_INPUT, '\n');

            switch (choice) {
                case 1:
                    user.registerUser();
                    break;
                case 2:
                    user.loginUser();
                    break;
                case 3:
                    cout << "Thank you for using CareSphere!" << endl;
                    return 0;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
} 