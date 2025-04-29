#define _CRT_SECURE_NO_WARNINGS
#include "../include/Doctor.h"
#include <iostream>
#include <iomanip>
#include <mysql.h>
#include "../include/Database.h"
#include <algorithm>
#include <regex>
#include <sstream>
#include <limits>

Doctor::Doctor() : User("doctor") {
    specialization = "";
    appointment_fees = 0.0;
    experience_years = 0;
}

bool Doctor::registerUser() {
    try {
        string name, email, password, phone, dob, specialization, license_number;
        double appointment_fees;
        int department_id;
        MYSQL* conn = Database::getConnection();
        MYSQL_RES* result = nullptr;

        cout << "\n=== Doctor Registration ===\n";
        
        // Name validation
        do {
            cout << "Enter name: ";
            getline(cin, name);
            if (name.length() < 2) {
                cout << "Name must be at least 2 characters long.\n";
            }
        } while (name.length() < 2);

        // Email validation
        do {
            cout << "Enter email: ";
            getline(cin, email);
            if (!isValidEmail(email)) {
                cout << "Invalid email format. Please enter a valid email.\n";
            }
        } while (!isValidEmail(email));

        // Password validation
        do {
            cout << "Enter password (min 8 characters, at least 1 uppercase, 1 lowercase, 1 number): ";
            getline(cin, password);
            if (!isValidPassword(password)) {
                cout << "Password must be at least 8 characters long and contain at least 1 uppercase letter, 1 lowercase letter, and 1 number.\n";
            }
        } while (!isValidPassword(password));

        // Phone validation
        do {
            cout << "Enter phone number (11 digits): ";
            getline(cin, phone);
            if (!isValidPhone(phone)) {
                cout << "Invalid phone number. Please enter 11 digits.\n";
            }
        } while (!isValidPhone(phone));

        // Date of birth validation
        do {
            cout << "Enter date of birth (YYYY-MM-DD): ";
            getline(cin, dob);
            if (!isValidDateOfBirth(dob)) {
                cout << "Invalid date of birth. Please enter a valid date in the past (YYYY-MM-DD).\n";
            }
        } while (!isValidDateOfBirth(dob));

        // License number validation
        do {
            cout << "Enter license number: ";
            getline(cin, license_number);
            if (license_number.length() < 5) {
                cout << "License number must be at least 5 characters long.\n";
            }
        } while (license_number.length() < 5);

        // Department selection
        cout << "\nAvailable Departments:\n";
        string query = "SELECT department_id, name FROM Departments ORDER BY name";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            return false;
        }
        result = mysql_store_result(conn);
        if (!result) {
            cout << "Error storing result: " << mysql_error(conn) << endl;
            return false;
        }
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << row[0] << ". " << row[1] << "\n";
        }
        mysql_free_result(result);

        do {
            cout << "Enter Department ID: ";
            if (!(cin >> department_id)) {
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }
            // Check if department exists
            query = "SELECT COUNT(*) FROM Departments WHERE department_id = " + to_string(department_id);
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error executing query: " << mysql_error(conn) << endl;
                return false;
            }
            result = mysql_store_result(conn);
            if (!result) {
                cout << "Error storing result: " << mysql_error(conn) << endl;
                return false;
            }
            row = mysql_fetch_row(result);
            if (atoi(row[0]) == 0) {
                cout << "Invalid department ID. Please select from the list.\n";
            }
            mysql_free_result(result);
        } while (atoi(row[0]) == 0);

        // Specialization validation
        do {
            cout << "Enter specialization (e.g., General Medicine, Cardiology, etc.): ";
            getline(cin, specialization);
            if (specialization.length() < 2) {
                cout << "Specialization must be at least 2 characters long.\n";
            }
        } while (specialization.length() < 2);

        // Appointment fees validation
        do {
            cout << "Enter appointment fees (100-5000): ";
            cin >> appointment_fees;
            cin.ignore();
            if (appointment_fees < 100 || appointment_fees > 5000) {
                cout << "Appointment fees must be between 100 and 5000 BDT.\n";
            }
        } while (appointment_fees < 100 || appointment_fees > 5000);

        // Degree input
        cout << "\nEnter number of degrees (0 if none): ";
        int num_degrees;
        cin >> num_degrees;
        cin.ignore();

        vector<string> degrees;
        for (int i = 0; i < num_degrees; i++) {
            string degree_name, institute;
            cout << "\nDegree " << (i + 1) << ":\n";
            cout << "Enter Degree Name: ";
            getline(cin, degree_name);
            cout << "Enter Institute: ";
            getline(cin, institute);
            
            if (degree_name.empty() || institute.empty()) {
                cout << "Degree name and institute cannot be empty!\n";
                i--; // Retry this degree
                continue;
            }
            
            degrees.push_back(degree_name + " from " + institute);
        }

        // Check if email already exists
        string check_query = "SELECT COUNT(*) FROM Users WHERE email = '" + email + "'";
        if (mysql_query(conn, check_query.c_str()) != 0) {
            cout << "Error checking email: " << mysql_error(conn) << endl;
            return false;
        }
        
        result = mysql_store_result(conn);
        row = mysql_fetch_row(result);
        if (atoi(row[0]) > 0) {
            cout << "Email already exists!\n";
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);
        
        // Check if phone already exists
        check_query = "SELECT COUNT(*) FROM Users WHERE phone = '" + phone + "'";
        if (mysql_query(conn, check_query.c_str()) != 0) {
            cout << "Error checking phone: " << mysql_error(conn) << endl;
            return false;
        }
        
        result = mysql_store_result(conn);
        row = mysql_fetch_row(result);
        if (atoi(row[0]) > 0) {
            cout << "Phone number already exists!\n";
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);

        // Hash the password
        string hashedPassword = hashPassword(password);
        cout << "Debug: Generated hash for password: " << hashedPassword << endl;

        // Start transaction
        mysql_query(conn, "START TRANSACTION");

        // Insert into Users table
        string user_query = "INSERT INTO Users (name, email, password, phone, dob, role) VALUES ('" +
                           name + "', '" + email + "', '" + hashedPassword + "', '" + phone + "', '" + 
                           dob + "', 'doctor')";
        
        cout << "Executing user registration query...\n";
        if (mysql_query(conn, user_query.c_str()) != 0) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }
        
        // Get the user_id of the newly inserted user
        int user_id = mysql_insert_id(conn);
        cout << "User registered successfully with ID: " << user_id << endl;
        
        // Insert into Doctors table
        string doctor_query = "INSERT INTO Doctors (user_id, department_id, specialization, appointment_fees, license_number) VALUES (" +
                             to_string(user_id) + ", " + to_string(department_id) + ", '" + 
                             specialization + "', " + to_string(appointment_fees) + ", '" + 
                             license_number + "')";
        
        cout << "Executing doctor registration query...\n";
        if (mysql_query(conn, doctor_query.c_str()) != 0) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }
        
        // Get the doctor_id of the newly inserted doctor
        int doctor_id = mysql_insert_id(conn);
        cout << "Doctor registered successfully with ID: " << doctor_id << endl;
        
        // Insert degrees
        for (const auto& degree : degrees) {
            // Split degree string into name and institute
            size_t pos = degree.find(" from ");
            string degree_name = degree.substr(0, pos);
            string institute = degree.substr(pos + 6); // 6 is length of " from "
            
            // Escape special characters
            char* escaped_degree = new char[degree_name.length() * 2 + 1];
            char* escaped_institute = new char[institute.length() * 2 + 1];
            mysql_real_escape_string(conn, escaped_degree, degree_name.c_str(), degree_name.length());
            mysql_real_escape_string(conn, escaped_institute, institute.c_str(), institute.length());
            
            string degree_query = "INSERT INTO DoctorDegrees (doctor_id, degree_name, institute) VALUES (" +
                                to_string(doctor_id) + ", '" + escaped_degree + "', '" + escaped_institute + "')";
            
            delete[] escaped_degree;
            delete[] escaped_institute;
            
            if (mysql_query(conn, degree_query.c_str()) != 0) {
                cout << "Error inserting degree: " << mysql_error(conn) << endl;
                mysql_query(conn, "ROLLBACK");
                return false;
            }
        }
        
        // Show available surgeries for the department and get surgery fees
        cout << "\nAvailable Surgeries for " << getDepartmentName(department_id) << ":\n";
        string surgery_query = "SELECT surgery_id, name FROM Surgeries WHERE department_id = " + to_string(department_id) + " ORDER BY name";
        if (mysql_query(conn, surgery_query.c_str()) != 0) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }
        result = mysql_store_result(conn);
        if (!result) {
            cout << "Error storing result: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }
        
        if (mysql_num_rows(result) == 0) {
            cout << "No surgeries found for this department.\n";
            mysql_free_result(result);
        } else {
            cout << "Enter surgery fees for each surgery (0 if not performing):\n";
            while ((row = mysql_fetch_row(result))) {
                double surgery_fee;
                do {
                    cout << row[1] << " (ID: " << row[0] << "): ";
                    if (!(cin >> surgery_fee)) {
                        cin.clear();
                        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                        cout << "Please enter a valid number.\n";
                        continue;
                    }
                    if (surgery_fee < 0) {
                        cout << "Surgery fee cannot be negative.\n";
                    }
                } while (surgery_fee < 0);

                if (surgery_fee > 0) {
                    string insert_surgery_query = "INSERT INTO DoctorSurgeryFees (doctor_id, surgery_id, fee) VALUES (" +
                                                to_string(doctor_id) + ", " + row[0] + ", " + to_string(surgery_fee) + ")";
                    if (mysql_query(conn, insert_surgery_query.c_str()) != 0) {
                        cout << "Error executing query: " << mysql_error(conn) << endl;
                        mysql_free_result(result);
                        mysql_query(conn, "ROLLBACK");
                        return false;
                    }
                }
            }
        }
        mysql_free_result(result);

        // Commit transaction
        mysql_query(conn, "COMMIT");
        return true;

    } catch (const std::exception& e) {
        // Rollback transaction on error
        MYSQL* conn = Database::getConnection();
        mysql_query(conn, "ROLLBACK");
        cout << "Error during registration: " << e.what() << "\n";
        return false;
    }
}

bool Doctor::updateProfile() {
    cout << "\n=== Update Doctor Profile ===\n";
    cout << "1. Update Phone\n";
    cout << "2. Update Specialization\n";
    cout << "3. Update Consultation Fee\n";
    cout << "4. Add Degree\n";
    cout << "5. Remove Degree\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    MYSQL* conn = Database::getConnection();
    string query;

    switch (choice) {
        case 1: {
            cout << "Enter new phone number: ";
            getline(cin, phone);
            if (!validatePhone(phone)) return false;
            query = "UPDATE Users SET phone = '" + phone + "' WHERE user_id = " + to_string(user_id);
            break;
        }
        case 2: {
            cout << "Enter new specialization: ";
            getline(cin, specialization);
            query = "UPDATE Doctors SET specialization = '" + specialization + 
                   "' WHERE user_id = " + to_string(user_id);
            break;
        }
        case 3: {
            cout << "Enter new consultation fee: ";
            cin >> appointment_fees;
            cin.ignore();
            query = "UPDATE Doctors SET appointment_fees = " + to_string(appointment_fees) + 
                   " WHERE user_id = " + to_string(user_id);
            break;
        }
        case 4:
            return addDegree("");
        case 5:
            return removeDegree("");
        default:
            cout << "Invalid choice!\n";
            return false;
    }

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Profile updated successfully!\n";
        return true;
    }
    return false;
}

void Doctor::displayInfo() const {
    cout << "\n=== Doctor Information ===\n";
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Date of Birth: " << dob << endl;
    cout << "Specialization: " << specialization << endl;
    cout << "Experience: " << experience_years << " years" << endl;
    cout << "Appointment Fee: $" << appointment_fees << endl;
    
    cout << "\nDegrees:\n";
    for (const auto& degree : degrees) {
        cout << "- " << degree << endl;
    }
    
    cout << "\nHospitals:\n";
    MYSQL* conn = Database::getConnection();
    string query = "SELECT h.hospital_name FROM Hospitals h "
                  "JOIN DoctorHospitals dh ON h.hospital_id = dh.hospital_id "
                  "WHERE dh.doctor_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << "- " << row[0] << endl;
        }
        mysql_free_result(result);
    }
}

bool Doctor::prescribeMedication(int patientId, const string& medicationName, 
                               const string& dosage, const string& duration) {
    MYSQL* conn = Database::getConnection();
    
    string query = "INSERT INTO Medications (patient_id, doctor_id, medication_name, dosage, duration, prescribed_date) "
                  "VALUES (" + to_string(patientId) + ", " + to_string(user_id) + ", '" + 
                  medicationName + "', '" + dosage + "', '" + duration + "', NOW())";
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Doctor::approveAppointment(int appointmentId) {
    MYSQL* conn = Database::getConnection();
    
    string query = "UPDATE Appointments SET status = 'approved' "
                  "WHERE appointment_id = " + to_string(appointmentId) + 
                  " AND doctor_id = " + to_string(user_id);
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Doctor::rejectAppointment(int appointmentId) {
    MYSQL* conn = Database::getConnection();
    
    string query = "UPDATE Appointments SET status = 'rejected' "
                  "WHERE appointment_id = " + to_string(appointmentId) + 
                  " AND doctor_id = " + to_string(user_id);
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Doctor::updateSchedule(const string& date, const string& time, bool isAvailable) {
    MYSQL* conn = Database::getConnection();
    
    string query = "INSERT INTO DoctorSchedule (doctor_id, date, time, is_available) "
                  "VALUES (" + to_string(user_id) + ", '" + date + "', '" + 
                  time + "', " + (isAvailable ? "1" : "0") + ")";
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Doctor::addHospital(int hospitalId) {
    hospital_ids.push_back(hospitalId);
    
    MYSQL* conn = Database::getConnection();
    string query = "INSERT INTO DoctorHospitals (doctor_id, hospital_id) VALUES (" +
                  to_string(user_id) + ", " + to_string(hospitalId) + ")";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Hospital added successfully!\n";
        return true;
    }
    return false;
}

bool Doctor::removeHospital(int hospitalId) {
    auto it = find(hospital_ids.begin(), hospital_ids.end(), hospitalId);
    if (it != hospital_ids.end()) {
        hospital_ids.erase(it);
        
        MYSQL* conn = Database::getConnection();
        string query = "DELETE FROM DoctorHospitals WHERE doctor_id = " + to_string(user_id) +
                      " AND hospital_id = " + to_string(hospitalId);
        
        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Hospital removed successfully!\n";
            return true;
        }
    }
    
    cout << "Hospital not found!\n";
    return false;
}

bool Doctor::addDegree(const string& degree) {
    degrees.push_back(degree);
    
    MYSQL* conn = Database::getConnection();
    string query = "INSERT INTO DoctorDegrees (doctor_id, degree_name) VALUES (" +
                  to_string(user_id) + ", '" + degree + "')";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Degree added successfully!\n";
        return true;
    }
    return false;
}

bool Doctor::removeDegree(const string& degree) {
    auto it = find(degrees.begin(), degrees.end(), degree);
    if (it != degrees.end()) {
        degrees.erase(it);
        
        MYSQL* conn = Database::getConnection();
        string query = "DELETE FROM DoctorDegrees WHERE doctor_id = " + to_string(user_id) +
                      " AND degree_name = '" + degree + "'";
        
        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Degree removed successfully!\n";
            return true;
        }
    }
    
    cout << "Degree not found!\n";
    return false;
}

bool Doctor::viewAppointments() const {
    MYSQL* conn = Database::getConnection();
    
    string query = "SELECT a.appointment_id, p.name as patient_name, h.hospital_name, "
                  "a.appointment_date, a.appointment_time, a.status "
                  "FROM Appointments a "
                  "JOIN Users p ON a.patient_id = p.user_id "
                  "JOIN DoctorHospitals dh ON a.doctor_id = dh.doctor_id "
                  "JOIN Hospitals h ON dh.hospital_id = h.hospital_id "
                  "WHERE a.doctor_id = " + to_string(user_id) +
                  " ORDER BY a.appointment_date, a.appointment_time";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        
        if (mysql_num_rows(result) == 0) {
            cout << "No appointments found!\n";
            mysql_free_result(result);
            return false;
        }

        cout << "\n=== Your Appointments ===\n";
        cout << left << setw(5) << "ID" << setw(20) << "Patient" << setw(30) << "Hospital"
             << setw(12) << "Date" << setw(10) << "Time" << "Status" << endl;
        cout << string(80, '-') << endl;

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << left << setw(5) << row[0] << setw(20) << row[1] << setw(30) << row[2]
                 << setw(12) << row[3] << setw(10) << row[4] << row[5] << endl;
        }

        mysql_free_result(result);
        return true;
    }
    return false;
}

// Helper functions for validation
bool Doctor::isValidEmail(const string& email) {
    const regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, email_pattern);
}

bool Doctor::isValidPassword(const string& password) {
    bool hasUpper = false;
    bool hasLower = false;
    bool hasNumber = false;
    
    if (password.length() < 8) return false;
    
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasNumber = true;
    }
    
    return hasUpper && hasLower && hasNumber;
}

bool Doctor::isValidPhone(const string& phone) {
    return phone.length() == 11 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool Doctor::isValidDateOfBirth(const string& dob) {
    try {
        // Parse the date
        tm tm = {};
        istringstream ss(dob);
        ss >> get_time(&tm, "%Y-%m-%d");
        if (ss.fail()) return false;

        // Convert to time_t for comparison
        time_t dob_time = mktime(&tm);
        time_t now = time(nullptr);

        // Check if date is in the past and reasonable for a doctor (at least 25 years old)
        const int MIN_AGE = 25;
        const int MAX_AGE = 80;
        double age = difftime(now, dob_time) / (365.25 * 24 * 60 * 60);
        
        return age >= MIN_AGE && age <= MAX_AGE;
    } catch (...) {
        return false;
    }
}

// Helper function to get department name
string Doctor::getDepartmentName(int department_id) {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT name FROM Departments WHERE department_id = " + to_string(department_id);
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        string name = row[0];
        mysql_free_result(result);
        return name;
    }
    return "Unknown Department";
} 