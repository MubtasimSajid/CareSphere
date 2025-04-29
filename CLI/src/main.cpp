#include "../include/User.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/Database.h"
#include "../include/PatientDashboard.h"
#include "../include/DoctorDashboard.h"
#include "../include/Hospital.h"
#include "../include/HospitalDashboard.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

void registerUser() {
    cout << "\n=== User Registration ===\n";
    cout << "1. Register as Patient\n";
    cout << "2. Register as Doctor\n";
    cout << "3. Register as Hospital\n";
    cout << "4. Back to Main Menu\n";
    cout << "Enter your choice (1-4): ";

    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        cout << "Invalid input. Please enter a number." << endl;
        return;
    }
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');

    switch (choice) {
        case 1: {
            Patient* patient = new Patient();
            if (patient->registerUser()) {
                cout << "\nPatient registered successfully!\n";
            } else {
                cout << "\nRegistration failed. Please try again.\n";
            }
            delete patient;
            break;
        }
        case 2: {
            Doctor* doctor = new Doctor();
            if (doctor->registerUser()) {
                cout << "\nDoctor registered successfully!\n";
            } else {
                cout << "\nRegistration failed. Please try again.\n";
            }
            delete doctor;
            break;
        }
        case 3: {
            Hospital* hospital = new Hospital();
            if (hospital->registerUser()) {
                cout << "\nHospital registered successfully!\n";
            } else {
                cout << "\nRegistration failed. Please try again.\n";
            }
            delete hospital;
            break;
        }
        case 4:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void loginUser() {
    string email, password;
    
    cout << "\n=== User Login ===\n";
    cout << "Enter email: ";
    getline(cin, email);
    
    cout << "Enter password: ";
    password = getSecurePassword();
    
    User* currentUser = nullptr;
    
    // First check user's role
    MYSQL* conn = Database::getConnection();
    string query = "SELECT role FROM Users WHERE email = '" + email + "'";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            string role = row[0];
            mysql_free_result(result);
            
            // Try logging in based on role
            if (role == "patient") {
                Patient* patient = new Patient();
                if (patient->login(email, password)) {
                    currentUser = patient;
                    cout << "\nLogin successful!\n";
                    PatientDashboard dashboard(patient->getUserId());
                    dashboard.displayDashboard();
                } else {
                    delete patient;
                    cout << "\nInvalid email or password!\n";
                    return;
                }
            } else if (role == "doctor") {
                Doctor* doctor = new Doctor();
                if (doctor->login(email, password)) {
                    currentUser = doctor;
                    cout << "\nLogin successful!\n";
                    DoctorDashboard dashboard(doctor->getUserId());
                    dashboard.displayDashboard();
                } else {
                    delete doctor;
                    cout << "\nInvalid email or password!\n";
                    return;
                }
            } else if (role == "hospital") {
                Hospital* hospital = new Hospital();
                if (hospital->login(email, password)) {
                    currentUser = hospital;
                    cout << "\nLogin successful!\n";
                    HospitalDashboard dashboard(hospital->getUserId());
                    dashboard.displayDashboard();
                } else {
                    delete hospital;
                    cout << "\nInvalid email or password!\n";
                    return;
                }
            } else {
                cout << "\nInvalid user role!\n";
                return;
            }
        } else {
            mysql_free_result(result);
            cout << "\nUser not found!\n";
            return;
        }
    } else {
        cout << "\nError checking user role: " << mysql_error(conn) << "\n";
        return;
    }
    
    delete currentUser;
}

int main() {
    try {
        // Get database connection
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cerr << "Failed to connect to database" << endl;
            return 1;
        }

        int choice;

        while (true) 
        {
            cout << "\n=== CareSphere Health Management System ===\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "Enter your choice (1-3): ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loginUser();
                    break;
                case 3:
                    cout << "Thank you for using CareSphere!\n";
                    return 0;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
} 