#include "../include/Patient.h"
#include <iostream>
#include <sstream>
#include <mysql.h>
#include "../include/Database.h"

Patient::Patient() : User("patient") {
    blood_group = "";
    gender = "";
    emergency_contact = "";
    medical_history = "";
    insurance_info = "";
    weight = 0.0;
    height = 0.0;
    blood_pressure = "";
    blood_sugar = 0.0;
}

bool Patient::registerUser() {
    string input;
    
    cout << "\n=== Patient Registration ===\n";
    
    // Get basic user information
    cout << "Enter name: ";
    getline(cin, input);
    setName(input);
    
    cout << "Enter email: ";
    getline(cin, input);
    setEmail(input);
    
    cout << "Enter password: ";
    getline(cin, input);
    setPassword(input);
    
    cout << "Enter phone number: ";
    getline(cin, input);
    setPhone(input);
    
    cout << "Enter date of birth (YYYY-MM-DD): ";
    getline(cin, input);
    setDOB(input);
    
    // Get patient-specific information
    cout << "Enter gender (male/female/transgender): ";
    getline(cin, input);
    setGender(input);
    
    cout << "Enter blood group (A+/A-/B+/B-/AB+/AB-/O+/O-): ";
    getline(cin, input);
    setBloodGroup(input);
    
    cout << "Enter emergency contact: ";
    getline(cin, input);
    setEmergencyContact(input);
    
    cout << "Enter medical history: ";
    getline(cin, input);
    setMedicalHistory(input);
    
    cout << "Enter insurance information: ";
    getline(cin, input);
    setInsuranceInfo(input);
    
    cout << "Enter weight (kg): ";
    double w;
    cin >> w;
    cin.ignore();
    setWeight(w);
    
    cout << "Enter height (cm): ";
    double h;
    cin >> h;
    cin.ignore();
    setHeight(h);
    
    cout << "Enter blood pressure (e.g., 120/80): ";
    getline(cin, input);
    setBloodPressure(input);
    
    cout << "Enter blood sugar: ";
    double bs;
    cin >> bs;
    cin.ignore();
    setBloodSugar(bs);
    
    // Get allergies
    cout << "Do you have any allergies? (y/n): ";
    char hasAllergies;
    cin >> hasAllergies;
    cin.ignore();
    
    if (tolower(hasAllergies) == 'y') {
        cout << "Enter allergies (one per line, press Enter twice when done):\n";
        string allergy;
        while (getline(cin, allergy) && !allergy.empty()) {
            addAllergy(allergy);
        }
    }
    
    // Get chronic conditions
    cout << "Do you have any chronic conditions? (y/n): ";
    char hasConditions;
    cin >> hasConditions;
    cin.ignore();
    
    if (tolower(hasConditions) == 'y') {
        cout << "Enter chronic conditions (one per line, press Enter twice when done):\n";
        string condition;
        while (getline(cin, condition) && !condition.empty()) {
            addChronicCondition(condition);
        }
    }
    
    // Register in database
    MYSQL* conn = Database::getConnection();
    if (!conn) {
        cout << "Error: Failed to connect to database\n";
        return false;
    }
    
    // First register as a user
    string query = "INSERT INTO Users (name, email, password, role, phone, dob) "
                  "VALUES ('" + name + "', '" + email + "', '" + password + "', 'patient', '" + 
                  phone + "', '" + dob + "')";
    
    cout << "Executing user registration query...\n";
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error registering user: " << mysql_error(conn) << "\n";
        return false;
    }
    
    user_id = mysql_insert_id(conn);
    cout << "User registered successfully with ID: " << user_id << "\n";
    
    // Then register as a patient
    query = "INSERT INTO Patients (user_id, sex, blood_group, weight, height, "
            "blood_pressure, blood_sugar, has_allergies, has_chronic_conditions) "
            "VALUES (" + to_string(user_id) + ", '" + gender + "', '" + blood_group + 
            "', " + to_string(weight) + ", " + to_string(height) + ", '" + 
            blood_pressure + "', " + to_string(blood_sugar) + ", " + 
            (allergies.empty() ? "0" : "1") + ", " + 
            (chronic_conditions.empty() ? "0" : "1") + ")";
    
    cout << "Executing patient registration query...\n";
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error registering patient: " << mysql_error(conn) << "\n";
        // Rollback user registration
        string rollback = "DELETE FROM Users WHERE user_id = " + to_string(user_id);
        mysql_query(conn, rollback.c_str());
        return false;
    }
    
    cout << "Patient registered successfully\n";
    
    // Get patient_id from Patients table
    query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(user_id);
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient_id: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        cout << "Error: Patient record not found\n";
        mysql_free_result(result);
        return false;
    }
    
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Insert allergies
    for (const auto& allergy : allergies) {
        query = "INSERT INTO PatientAllergies (patient_id, allergy_name) VALUES (" +
               to_string(patientId) + ", '" + allergy + "')";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Warning: Failed to insert allergy: " << mysql_error(conn) << "\n";
        }
    }
    
    // Insert chronic conditions
    for (const auto& condition : chronic_conditions) {
        query = "INSERT INTO PatientChronicConditions (patient_id, condition_name) VALUES (" +
               to_string(patientId) + ", '" + condition + "')";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Warning: Failed to insert chronic condition: " << mysql_error(conn) << "\n";
        }
    }
    
    cout << "Registration completed successfully!\n";
    return true;
}

bool Patient::updateProfile() {
    cout << "\n=== Update Patient Profile ===\n";
    cout << "1. Update Phone\n";
    cout << "2. Update Emergency Contact\n";
    cout << "3. Update Medical History\n";
    cout << "4. Update Insurance Info\n";
    cout << "5. Update Weight/Height\n";
    cout << "6. Update Blood Pressure/Sugar\n";
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
            cout << "Enter new emergency contact: ";
            getline(cin, emergency_contact);
            if (!validatePhone(emergency_contact)) return false;
            query = "UPDATE Patients SET emergency_contact = '" + emergency_contact + 
                   "' WHERE user_id = " + to_string(user_id);
            break;
        }
        case 3: {
            cout << "Enter new medical history: ";
            getline(cin, medical_history);
            query = "UPDATE Patients SET medical_history = '" + medical_history + 
                   "' WHERE user_id = " + to_string(user_id);
            break;
        }
        case 4: {
            cout << "Enter new insurance information: ";
            getline(cin, insurance_info);
            query = "UPDATE Patients SET insurance_info = '" + insurance_info + 
                   "' WHERE user_id = " + to_string(user_id);
            break;
        }
        case 5: {
            cout << "Enter new weight (kg): ";
            cin >> weight;
            cout << "Enter new height (cm): ";
            cin >> height;
            cin.ignore();
            query = "UPDATE Patients SET weight = " + to_string(weight) + 
                   ", height = " + to_string(height) + 
                   " WHERE user_id = " + to_string(user_id);
            break;
        }
        case 6: {
            cout << "Enter new blood pressure: ";
            getline(cin, blood_pressure);
            cout << "Enter new blood sugar: ";
            cin >> blood_sugar;
            cin.ignore();
            query = "UPDATE Patients SET blood_pressure = '" + blood_pressure + 
                   "', blood_sugar = " + to_string(blood_sugar) + 
                   " WHERE user_id = " + to_string(user_id);
            break;
        }
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

void Patient::displayInfo() const {
    cout << "\n=== Patient Information ===\n";
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Date of Birth: " << dob << endl;
    cout << "Gender: " << gender << endl;
    cout << "Blood Group: " << blood_group << endl;
    cout << "Emergency Contact: " << emergency_contact << endl;
    cout << "Weight: " << weight << " kg" << endl;
    cout << "Height: " << height << " cm" << endl;
    cout << "Blood Pressure: " << blood_pressure << endl;
    cout << "Blood Sugar: " << blood_sugar << endl;
    
    cout << "\nAllergies:\n";
    for (const auto& allergy : allergies) {
        cout << "- " << allergy << endl;
    }
    
    cout << "\nChronic Conditions:\n";
    for (const auto& condition : chronic_conditions) {
        cout << "- " << condition << endl;
    }
}

bool Patient::bookAppointment(int doctorId, const string& date, const string& time) {
    MYSQL* conn = Database::getConnection();
    
    string query = "INSERT INTO Appointments (patient_id, doctor_id, appointment_date, "
                  "appointment_time, status) VALUES (" + to_string(user_id) + ", " + 
                  to_string(doctorId) + ", '" + date + "', '" + time + "', 'pending')";
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Patient::cancelAppointment(int appointmentId) {
    MYSQL* conn = Database::getConnection();
    
    string query = "UPDATE Appointments SET status = 'cancelled' "
                  "WHERE appointment_id = " + to_string(appointmentId) + 
                  " AND patient_id = " + to_string(user_id);
    
    return mysql_query(conn, query.c_str()) == 0;
}

bool Patient::viewAppointments() const {
    MYSQL* conn = Database::getConnection();
    
    string query = "SELECT a.appointment_id, d.name, a.appointment_date, "
                  "a.appointment_time, a.status FROM Appointments a "
                  "JOIN Doctors d ON a.doctor_id = d.user_id "
                  "WHERE a.patient_id = " + to_string(user_id) +
                  " ORDER BY a.appointment_date, a.appointment_time";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        cout << "\n=== Your Appointments ===\n";
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << "Appointment ID: " << row[0] << endl;
            cout << "Doctor: " << row[1] << endl;
            cout << "Date: " << row[2] << endl;
            cout << "Time: " << row[3] << endl;
            cout << "Status: " << row[4] << endl;
            cout << "-------------------\n";
        }
        mysql_free_result(result);
        return true;
    }
    return false;
}

bool Patient::updateMedicalHistory(const string& history) {
    medical_history = history;
    return updateProfile();
}

int Patient::calculateAge(const string& dob) const {
    // Parse date of birth
    int year = std::stoi(dob.substr(0, 4));
    int month = std::stoi(dob.substr(5, 2));
    int day = std::stoi(dob.substr(8, 2));
    
    // Get current date
    time_t now = std::time(nullptr);
    struct tm ltm;
    localtime_s(&ltm, &now);
    
    // Calculate age
    int age = 1900 + ltm.tm_year - year;
    if (ltm.tm_mon + 1 < month || (ltm.tm_mon + 1 == month && ltm.tm_mday < day)) {
        age--;
    }
    
    return age;
} 