#define _CRT_SECURE_NO_WARNINGS
#include "../include/User.h"
#include "../include/Common.h"
#include <iostream>
#include <string>
#include <regex>
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * @brief Validates user input based on type
 * @param name The name to validate
 * @param email The email to validate
 * @param phone The phone number to validate
 * @param password The password to validate
 * @return true if all inputs are valid, false otherwise
 */
bool User::validateInput(const string& input, const string& type) {
    if (input.empty()) {
        cout << type << " cannot be empty!" << endl;
        return false;
    }

    if (type == "name") {
        regex namePattern("[A-Za-z ]+");
        if (!regex_match(input, namePattern)) {
            cout << "Invalid name format! Use only letters and spaces." << endl;
            return false;
        }
    }
    else if (type == "email") {
        regex emailPattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!regex_match(input, emailPattern)) {
            cout << "Invalid email format!" << endl;
            return false;
        }
    }
    else if (type == "phone") {
        regex phonePattern("^[0-9]{11}$");
        if (!regex_match(input, phonePattern)) {
            cout << "Invalid phone format! Use exactly 11 digits." << endl;
            return false;
        }
    }
    else if (type == "password") {
        if (input.length() < 8) {
            cout << "Password must be at least 8 characters long!" << endl;
            return false;
        }
    }
    else if (type == "date") {
        regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");
        if (!regex_match(input, datePattern)) {
            cout << "Invalid date format! Use YYYY-MM-DD." << endl;
            return false;
        }
    }

    return true;
}

int User::calculateAge(const string& dob) {
    tm birth = {};
    stringstream ss(dob);
    ss >> get_time(&birth, "%Y-%m-%d");
    
    time_t now = time(nullptr);
    tm* current = localtime(&now);
    
    int age = current->tm_year + 1900 - (birth.tm_year + 1900);
    if (current->tm_mon < birth.tm_mon || 
        (current->tm_mon == birth.tm_mon && current->tm_mday < birth.tm_mday)) {
        age--;
    }
    return age;
}

bool User::validateBloodGroup(const string& bloodGroup) {
    vector<string> validGroups = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    return find(validGroups.begin(), validGroups.end(), bloodGroup) != validGroups.end();
}

bool User::validateGender(const string& gender) {
    vector<string> validGenders = {"male", "female", "transgender"};
    string lowerGender = gender;
    transform(lowerGender.begin(), lowerGender.end(), lowerGender.begin(), ::tolower);
    return find(validGenders.begin(), validGenders.end(), lowerGender) != validGenders.end();
}

vector<string> User::getInputList(const string& prompt) {
    vector<string> items;
    string input;
    cout << prompt << " (Enter 'done' when finished):\n";
    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input == "done") break;
        if (!input.empty()) items.push_back(input);
    }
    return items;
}

bool User::registerPatient(MYSQL* conn, int userId) {
    string gender, bloodGroup, weight, height, bloodPressure, bloodSugar;
    char hasAllergies, hasConditions;
    
    cout << "\nPatient Registration\n";
    cout << "Enter Gender (male/female/transgender): ";
    getline(cin, gender);
    if (!validateGender(gender)) return false;
    
    cout << "Enter Blood Group: ";
    getline(cin, bloodGroup);
    if (!validateBloodGroup(bloodGroup)) {
        cout << "Invalid blood group! Valid groups are: A+, A-, B+, B-, AB+, AB-, O+, O-\n";
        return false;
    }
    
    cout << "Enter Weight (kg): ";
    getline(cin, weight);
    cout << "Enter Height (cm): ";
    getline(cin, height);
    cout << "Enter Blood Pressure (e.g., 120/80): ";
    getline(cin, bloodPressure);
    cout << "Enter Blood Sugar: ";
    getline(cin, bloodSugar);
    
    cout << "Do you have any allergies? (y/n): ";
    cin >> hasAllergies;
    cin.ignore();
    
    vector<string> allergies;
    if (tolower(hasAllergies) == 'y') {
        allergies = getInputList("Enter your allergies");
    }
    
    cout << "Do you have any chronic conditions? (y/n): ";
    cin >> hasConditions;
    cin.ignore();
    
    vector<string> conditions;
    if (tolower(hasConditions) == 'y') {
        conditions = getInputList("Enter your chronic conditions");
    }

    // Insert patient record
    string query = "INSERT INTO Patients (user_id, sex, blood_group, weight, height, "
                  "blood_pressure, blood_sugar, has_allergies, has_chronic_conditions) VALUES ("
                  + to_string(userId) + ", '" + gender + "', '" + bloodGroup + "', "
                  + weight + ", " + height + ", '" + bloodPressure + "', " + bloodSugar + ", "
                  + (hasAllergies == 'y' ? "1" : "0") + ", "
                  + (hasConditions == 'y' ? "1" : "0") + ")";
    MYSQL_CHECK_ERROR(conn, query);
    
    int patientId = mysql_insert_id(conn);
    
    // Insert allergies if any
    for (const auto& allergy : allergies) {
        query = "INSERT INTO PatientAllergies (patient_id, allergy_name) VALUES ("
                + to_string(patientId) + ", '" + allergy + "')";
        MYSQL_CHECK_ERROR(conn, query);
    }
    
    // Insert chronic conditions if any
    for (const auto& condition : conditions) {
        query = "INSERT INTO PatientChronicConditions (patient_id, condition_name) VALUES ("
                + to_string(patientId) + ", '" + condition + "')";
        MYSQL_CHECK_ERROR(conn, query);
    }
    
    return true;
}

bool User::validateDepartment(MYSQL* conn, int departmentId) {
    string query = "SELECT COUNT(*) FROM Departments WHERE department_id = " + to_string(departmentId);
    MYSQL_CHECK_ERROR(conn, query);
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = (row && stoi(row[0]) > 0);
    mysql_free_result(result);
    return exists;
}

bool User::registerDoctor(MYSQL* conn, int userId) {
    int departmentId, numDegrees;
    string specialization;
    double appointmentFees;
    
    cout << "\nDoctor Registration\n";
    
    // Display available departments
    cout << "\nAvailable Departments:\n";
    string query = "SELECT department_id, name FROM Departments";
    MYSQL_CHECK_ERROR(conn, query);
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << row[0] << ". " << row[1] << endl;
    }
    mysql_free_result(result);
    
    cout << "Enter Department ID: ";
    cin >> departmentId;
    cin.ignore();
    if (!validateDepartment(conn, departmentId)) {
        cout << "Invalid department ID!\n";
        return false;
    }
    
    cout << "Enter Specialization: ";
    getline(cin, specialization);
    
    cout << "Enter Appointment Fees: ";
    cin >> appointmentFees;
    cin.ignore();
    
    cout << "Enter number of degrees: ";
    cin >> numDegrees;
    cin.ignore();
    
    // Insert doctor record
    query = "INSERT INTO Doctors (user_id, department_id, specialization, appointment_fees) VALUES ("
            + to_string(userId) + ", " + to_string(departmentId) + ", '"
            + specialization + "', " + to_string(appointmentFees) + ")";
    MYSQL_CHECK_ERROR(conn, query);
    
    int doctorId = mysql_insert_id(conn);
    
    // Insert degrees
    for (int i = 0; i < numDegrees; i++) {
        string degreeName, institute;
        cout << "\nDegree " << (i + 1) << ":\n";
        cout << "Enter Degree Name: ";
        getline(cin, degreeName);
        cout << "Enter Institute: ";
        getline(cin, institute);
        
        query = "INSERT INTO DoctorDegrees (doctor_id, degree_name, institute) VALUES ("
                + to_string(doctorId) + ", '" + degreeName + "', '" + institute + "')";
        MYSQL_CHECK_ERROR(conn, query);
    }
    
    // Set surgery fees
    cout << "\nSet fees for surgeries in your department:\n";
    query = "SELECT surgery_id, name FROM Surgeries WHERE department_id = " + to_string(departmentId);
    MYSQL_CHECK_ERROR(conn, query);
    
    result = mysql_store_result(conn);
    while ((row = mysql_fetch_row(result))) {
        double surgeryFee;
        cout << "Enter fee for " << row[1] << ": ";
        cin >> surgeryFee;
        cin.ignore();
        
        query = "INSERT INTO DoctorSurgeryFees (doctor_id, surgery_id, fee) VALUES ("
                + to_string(doctorId) + ", " + row[0] + ", " + to_string(surgeryFee) + ")";
        MYSQL_CHECK_ERROR(conn, query);
    }
    mysql_free_result(result);
    
    return true;
}

bool User::registerHospital(MYSQL* conn, int userId) {
    string hospitalName, address, city, state, country, pincode;
    int numDepartments;
    
    cout << "\nHospital Registration\n";
    cout << "Enter Hospital Name: ";
    getline(cin, hospitalName);
    
    cout << "Enter Address: ";
    getline(cin, address);
    
    cout << "Enter City: ";
    getline(cin, city);
    
    cout << "Enter State: ";
    getline(cin, state);
    
    cout << "Enter Country: ";
    getline(cin, country);
    
    cout << "Enter Pincode: ";
    getline(cin, pincode);
    
    // Insert hospital record
    string query = "INSERT INTO Hospitals (user_id, name, address, city, state, country, pincode) VALUES ("
            + to_string(userId) + ", '" + hospitalName + "', '" + address + "', '"
            + city + "', '" + state + "', '" + country + "', '" + pincode + "')";
    MYSQL_CHECK_ERROR(conn, query);
    
    int hospitalId = mysql_insert_id(conn);
    
    cout << "Enter number of departments to add: ";
    cin >> numDepartments;
    cin.ignore();
    
    for (int i = 0; i < numDepartments; i++) {
        string deptName, description;
        cout << "\nDepartment " << (i + 1) << ":\n";
        cout << "Enter Department Name: ";
        getline(cin, deptName);
        cout << "Enter Description: ";
        getline(cin, description);
        
        query = "INSERT INTO Departments (hospital_id, name, description) VALUES ("
                + to_string(hospitalId) + ", '" + deptName + "', '" + description + "')";
        MYSQL_CHECK_ERROR(conn, query);
        
        int departmentId = mysql_insert_id(conn);
        
        // Add surgeries for this department
        cout << "Enter number of surgeries offered in this department: ";
        int numSurgeries;
        cin >> numSurgeries;
        cin.ignore();
        
        for (int j = 0; j < numSurgeries; j++) {
            string surgeryName, surgeryDesc;
            cout << "\nSurgery " << (j + 1) << ":\n";
            cout << "Enter Surgery Name: ";
            getline(cin, surgeryName);
            cout << "Enter Surgery Description: ";
            getline(cin, surgeryDesc);
            
            query = "INSERT INTO Surgeries (department_id, name, description) VALUES ("
                    + to_string(departmentId) + ", '" + surgeryName + "', '" + surgeryDesc + "')";
            MYSQL_CHECK_ERROR(conn, query);
        }
    }
    
    return true;
}

bool User::registerUser() {
    try {
        string name, email, phone, password, dob;
        
        cout << "\n=== User Registration ===\n";
        cout << "Enter Name: ";
        getline(cin, name);
        if (!validateInput(name, "name")) return false;
        
        cout << "Enter Email: ";
        getline(cin, email);
        if (!validateInput(email, "email")) return false;
        
        cout << "Enter Phone (11 digits): ";
        getline(cin, phone);
        if (!validateInput(phone, "phone")) return false;
        
        cout << "Enter Password: ";
        getline(cin, password);
        if (!validateInput(password, "password")) return false;
        
        cout << "Enter Date of Birth (YYYY-MM-DD): ";
        getline(cin, dob);
        if (!validateInput(dob, "date")) return false;
        
        int age = calculateAge(dob);
        
        cout << "Select Role:\n";
        cout << "1. Patient\n";
        if (age >= 18) {
            cout << "2. Doctor\n";
            cout << "3. Hospital Admin\n";
        }
        cout << "Enter choice: ";
        
        int roleChoice;
        cin >> roleChoice;
        cin.ignore();
        
        string role;
        if (age < 18 && roleChoice != 1) {
            cout << "You must be at least 18 years old to register as a doctor or hospital admin.\n";
            return false;
        }
        
        switch (roleChoice) {
            case 1: role = "patient"; break;
            case 2: role = "doctor"; break;
            case 3: role = "hospital"; break;
            default:
                cout << "Invalid role selection!" << endl;
                return false;
        }

        MYSQL* conn = Database::getConnection();
        if (!conn) {
            throw runtime_error("Database connection failed");
        }

        // Check if email already exists
        string query = "SELECT COUNT(*) FROM Users WHERE email = '" + email + "'";
        MYSQL_CHECK_ERROR(conn, query);

        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row && stoi(row[0]) > 0) {
            cout << "Email already registered!" << endl;
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);
        
        // Check if phone already exists
        query = "SELECT COUNT(*) FROM Users WHERE phone = '" + phone + "'";
        MYSQL_CHECK_ERROR(conn, query);
        
        result = mysql_store_result(conn);
        row = mysql_fetch_row(result);
        if (row && stoi(row[0]) > 0) {
            cout << "Phone number already registered!" << endl;
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);

        // Insert new user
        string hashedPassword = hashPassword(password);
        query = "INSERT INTO Users (name, email, password, phone, date_of_birth, role) VALUES ('" +
                name + "', '" + email + "', '" + hashedPassword + "', '" + phone + "', '" + dob + "', '" + role + "')";
        MYSQL_CHECK_ERROR(conn, query);

        int userId = mysql_insert_id(conn);
        bool success = false;

        // Handle role-specific registration
        if (role == "patient") {
            success = registerPatient(conn, userId);
        }
        else if (role == "doctor") {
            success = registerDoctor(conn, userId);
        }
        else if (role == "hospital") {
            success = registerHospital(conn, userId);
        }

        if (!success) {
            // Rollback user creation if role-specific registration failed
            query = "DELETE FROM Users WHERE user_id = " + to_string(userId);
            mysql_query(conn, query.c_str());
            return false;
        }

        cout << "\nRegistration successful!\n";
        return true;

    } catch (const exception& e) {
        cerr << "Registration error: " << e.what() << endl;
        return false;
    }
}

bool User::loginUser() {
    try {
        string email, password;
        
        cout << "\n=== User Login ===\n";
        cout << "Enter Email: ";
        getline(cin, email);
        if (!validateInput(email, "email")) return false;

        cout << "Enter Password: ";
        getline(cin, password);
        if (!validateInput(password, "password")) return false;

        MYSQL* conn = Database::getConnection();
        if (!conn) {
            throw runtime_error("Database connection failed");
        }

        string hashedPassword = hashPassword(password);
        string query = "SELECT user_id, name, role FROM Users "
                       "WHERE email = '" + email + "' AND password = '" + hashedPassword + "'";
        MYSQL_CHECK_ERROR(conn, query);

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) {
            throw runtime_error("Error retrieving results: " + string(mysql_error(conn)));
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        if (!row) {
            mysql_free_result(res);
            cout << "Invalid email or password" << endl;
            return false;
        }

        int userId = stoi(row[0]);
        string name = row[1];
        string role = row[2];
        mysql_free_result(res);

        cout << "\nLogin successful!\n";
        cout << "Welcome, " << name << "!\n";
        cout << "Role: " << role << "\n";

        return true;

    } catch (const exception& e) {
        cerr << "Login error: " << e.what() << endl;
        return false;
    }
}

string User::hashPassword(const string& password) {
    // This is a simple hash function for demonstration
    // In a real application, use a proper cryptographic hash function
    hash<string> hasher;
    return to_string(hasher(password));
} 