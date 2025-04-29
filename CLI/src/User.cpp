#define _CRT_SECURE_NO_WARNINGS
#include "../include/User.h"
#include "../include/Common.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include "../include/PatientDashboard.h"
#include "../include/DoctorDashboard.h"
#include <iostream>
#include <string>
#include <regex>
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <mysql.h>

using namespace std;



bool User::validateInput(const string& input, const string& type) 
{
    if (input.empty()) 
    {
        cout << type << " cannot be empty!" << endl;
        return false;
    }

    if (type == "name") 
    {
        regex namePattern("[A-Za-z ]+");
        if (!regex_match(input, namePattern)) 
        {
            cout << "Invalid name format! Use only letters and spaces." << endl;
            return false;
        }
    }
    else if (type == "email") 
    {
        regex emailPattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!regex_match(input, emailPattern)) 
        {
            cout << "Invalid email format!" << endl;
            return false;
        }
    }
    else if (type == "phone") 
    {
        regex phonePattern("^[0-9]{11}$");
        if (!regex_match(input, phonePattern)) 
        {
            cout << "Invalid phone format! Use exactly 11 digits." << endl;
            return false;
        }
    }
    else if (type == "password") 
    {
        if (input.length() < 8) 
        {
            cout << "Password must be at least 8 characters long!" << endl;
            return false;
        }
    }
    else if (type == "date") 
    {
        regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");
        if (!regex_match(input, datePattern)) 
        {
            cout << "Invalid date format! Use YYYY-MM-DD." << endl;
            return false;
        }
    }

    return true;
}

int User::calculateAge(const string& dob) 
{
    tm birth = {};
    stringstream ss(dob);
    ss >> get_time(&birth, "%Y-%m-%d");
    
    time_t now = time(nullptr);
    tm* current = localtime(&now);
    
    int age = current->tm_year + 1900 - (birth.tm_year + 1900);
    if (current->tm_mon < birth.tm_mon || 
        (current->tm_mon == birth.tm_mon && current->tm_mday < birth.tm_mday)) 
    {
        age--;
    }
    return age;
}

bool User::validateBloodGroup(const string& bloodGroup) 
{
    vector<string> validGroups = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    return find(validGroups.begin(), validGroups.end(), bloodGroup) != validGroups.end();
}

bool User::validateGender(const string& gender) 
{
    vector<string> validGenders = {"male", "female", "transgender"};
    string lowerGender = gender;
    transform(lowerGender.begin(), lowerGender.end(), lowerGender.begin(), ::tolower);
    return find(validGenders.begin(), validGenders.end(), lowerGender) != validGenders.end();
}

vector<string> User::getInputList(const string& prompt) 
{
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

bool User::registerPatient(MYSQL* conn, int userId) 
{
    string gender, bloodGroup, weight, height, bloodPressure, bloodSugar;
    char hasAllergies, hasConditions;
    
    cout << "\nPatient Registration\n";
    cout << "Enter Gender (male/female/transgender): ";
    getline(cin, gender);
    if (!validateGender(gender)) return false;
    
    cout << "Enter Blood Group: ";
    getline(cin, bloodGroup);
    if (!validateBloodGroup(bloodGroup)) 
    {
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
    if (tolower(hasAllergies) == 'y') 
    {
        allergies = getInputList("Enter your allergies");
    }
    
    cout << "Do you have any chronic conditions? (y/n): ";
    cin >> hasConditions;
    cin.ignore();
    
    vector<string> conditions;
    if (tolower(hasConditions) == 'y') 
    {
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
    for (const auto& condition : conditions) 
    {
        query = "INSERT INTO PatientChronicConditions (patient_id, condition_name) VALUES ("
                + to_string(patientId) + ", '" + condition + "')";
        MYSQL_CHECK_ERROR(conn, query);
    }
    
    return true;
}

bool User::validateDepartment(MYSQL* conn, int departmentId) 
{
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
    
    //  insert the doctor record
    query = "INSERT INTO Doctors (user_id, department_id, specialization, appointment_fees) VALUES ("
            + to_string(userId) + ", " + to_string(departmentId) + ", '"
            + specialization + "', " + to_string(appointmentFees) + ")";
    MYSQL_CHECK_ERROR(conn, query);
    
    int doctorId = mysql_insert_id(conn);
    if (doctorId == 0) {
        cout << "Failed to create doctor record!\n";
        return false;
    }
    
    //  insert degrees
    for (int i = 0; i < numDegrees; i++) {
        string degreeName, institute;
        cout << "\nDegree " << (i + 1) << ":\n";
        cout << "Enter Degree Name: ";
        getline(cin, degreeName);
        cout << "Enter Institute: ";
        getline(cin, institute);
        
        query = "INSERT INTO DoctorDegrees (doctor_id, degree_name, institute) VALUES ("
                + to_string(doctorId) + ", '" + degreeName + "', '" + institute + "')";
        if (!Database::executeQuery(query)) {
            cout << "Failed to add degree!\n";
            return false;
        }
    }
    
    //  set surgery fees
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
        if (!Database::executeQuery(query)) {
            cout << "Failed to set surgery fee!\n";
            mysql_free_result(result);
            return false;
        }
    }
    mysql_free_result(result);
    
    return true;
}

bool User::registerHospital(MYSQL* conn, int userId) {
    string hospitalName, address, city, state, country, pincode;
    bool hasAmbulance;
    
    cout << "\n=== Hospital Registration ===\n";
    
    // Input validation for hospital name
    do {
        cout << "Enter Hospital Name: ";
        getline(cin, hospitalName);
        if (hospitalName.empty()) {
            cout << "Hospital name cannot be empty. Please try again.\n";
        }
    } while (hospitalName.empty());
    
    // Input validation for address
    do {
        cout << "Enter Address: ";
        getline(cin, address);
        if (address.empty()) {
            cout << "Address cannot be empty. Please try again.\n";
        }
    } while (address.empty());
    
    // Input validation for city
    do {
        cout << "Enter City: ";
        getline(cin, city);
        if (city.empty()) {
            cout << "City cannot be empty. Please try again.\n";
        }
    } while (city.empty());
    
    // Input validation for state
    do {
        cout << "Enter State: ";
        getline(cin, state);
        if (state.empty()) {
            cout << "State cannot be empty. Please try again.\n";
        }
    } while (state.empty());
    
    // Input validation for country
    do {
        cout << "Enter Country: ";
        getline(cin, country);
        if (country.empty()) {
            cout << "Country cannot be empty. Please try again.\n";
        }
    } while (country.empty());
    
    // Input validation for pincode
    do {
        cout << "Enter Pincode: ";
        getline(cin, pincode);
        if (pincode.empty() || pincode.length() > 10) {
            cout << "Invalid pincode. Please enter a valid pincode (max 10 characters).\n";
        }
    } while (pincode.empty() || pincode.length() > 10);

    cout << "Does the hospital have ambulance service? (1 for yes, 0 for no): ";
    cin >> hasAmbulance;
    cin.ignore();
    
    // Escape special characters in strings
    char* escapedName = new char[hospitalName.length() * 2 + 1];
    char* escapedAddress = new char[address.length() * 2 + 1];
    char* escapedCity = new char[city.length() * 2 + 1];
    char* escapedState = new char[state.length() * 2 + 1];
    char* escapedCountry = new char[country.length() * 2 + 1];
    
    mysql_real_escape_string(conn, escapedName, hospitalName.c_str(), hospitalName.length());
    mysql_real_escape_string(conn, escapedAddress, address.c_str(), address.length());
    mysql_real_escape_string(conn, escapedCity, city.c_str(), city.length());
    mysql_real_escape_string(conn, escapedState, state.c_str(), state.length());
    mysql_real_escape_string(conn, escapedCountry, country.c_str(), country.length());
    
    // Insert hospital record with proper escaping
    string query = "INSERT INTO Hospitals (user_id, hospital_name, address, city, state, country, pincode, has_ambulance) VALUES ("
            + to_string(userId) + ", '" + escapedName + "', '" + escapedAddress + "', '"
            + escapedCity + "', '" + escapedState + "', '" + escapedCountry + "', '" + pincode + "', "
            + (hasAmbulance ? "1" : "0") + ")";
    
    delete[] escapedName;
    delete[] escapedAddress;
    delete[] escapedCity;
    delete[] escapedState;
    delete[] escapedCountry;
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error registering hospital: " << mysql_error(conn) << "\n";
        return false;
    }
    
    int hospitalId = mysql_insert_id(conn);
    cout << "\nHospital registered successfully with ID: " << hospitalId << "\n";

    // Handle ambulance information if available
    if (hasAmbulance) {
        cout << "\n=== Ambulance Service Setup ===\n";
        cout << "Enter number of ambulances: ";
        int numAmbulances;
        cin >> numAmbulances;
        cin.ignore();

        if (numAmbulances > 0) {
            for (int i = 0; i < numAmbulances; i++) {
                string vehicleNumber, phoneNumber;
                double feePerKm;
                
                cout << "\nAmbulance " << (i + 1) << ":\n";
                
                // Validate vehicle number
                do {
                    cout << "Enter Vehicle Number: ";
                    getline(cin, vehicleNumber);
                    if (vehicleNumber.empty()) {
                        cout << "Vehicle number cannot be empty. Please try again.\n";
                    }
                } while (vehicleNumber.empty());
                
                // Validate phone number
                do {
                    cout << "Enter Phone Number: ";
                    getline(cin, phoneNumber);
                    if (phoneNumber.empty() || phoneNumber.length() > 15) {
                        cout << "Invalid phone number. Please enter a valid phone number (max 15 characters).\n";
                    }
                } while (phoneNumber.empty() || phoneNumber.length() > 15);
                
                // Validate fee
                do {
                    cout << "Enter Fee per Kilometer: ";
                    cin >> feePerKm;
                    if (feePerKm <= 0) {
                        cout << "Fee must be greater than 0. Please try again.\n";
                    }
                } while (feePerKm <= 0);
                cin.ignore();

                // Escape special characters
                char* escapedVehicleNumber = new char[vehicleNumber.length() * 2 + 1];
                char* escapedPhoneNumber = new char[phoneNumber.length() * 2 + 1];
                
                mysql_real_escape_string(conn, escapedVehicleNumber, vehicleNumber.c_str(), vehicleNumber.length());
                mysql_real_escape_string(conn, escapedPhoneNumber, phoneNumber.c_str(), phoneNumber.length());

                query = "INSERT INTO HospitalAmbulances (hospital_id, vehicle_number, phone_number, fee_per_km) VALUES (" +
                    to_string(hospitalId) + ", '" + escapedVehicleNumber + "', '" + escapedPhoneNumber + "', " + to_string(feePerKm) + ")";
                
                delete[] escapedVehicleNumber;
                delete[] escapedPhoneNumber;
                
                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error adding ambulance: " << mysql_error(conn) << "\n";
                    return false;
                }
            }
            cout << "\nAmbulance service setup completed successfully.\n";
        }
    }
    
    // Handle departments
    cout << "\n=== Department Setup ===\n";
    cout << "Available Departments:\n";
    query = "SELECT department_id, name FROM Departments";
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching departments: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        cout << row[0] << ". " << row[1] << endl;
    }
    mysql_free_result(res);

    cout << "\nEnter number of departments to add: ";
    int numDepartments;
    cin >> numDepartments;
    cin.ignore();

    for (int i = 0; i < numDepartments; i++) {
        int departmentId;
        string departmentName, description;
        double appointmentFee;
        
        cout << "\nDepartment " << (i + 1) << ":\n";
        
        // Validate department ID
        do {
            cout << "Enter Department ID: ";
            cin >> departmentId;
            cin.ignore();
            
            // Check if department exists
            query = "SELECT COUNT(*) FROM Departments WHERE department_id = " + to_string(departmentId);
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error checking department: " << mysql_error(conn) << "\n";
                return false;
            }
            
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            if (stoi(row[0]) == 0) {
                cout << "Invalid department ID. Please try again.\n";
            }
            mysql_free_result(res);
        } while (stoi(row[0]) == 0);
        
        // Get department name from Departments table
        query = "SELECT name FROM Departments WHERE department_id = " + to_string(departmentId);
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching department name: " << mysql_error(conn) << "\n";
            return false;
        }
        
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        departmentName = row[0];
        mysql_free_result(res);
        
        cout << "Enter Description: ";
        getline(cin, description);
        
        // Validate appointment fee
        do {
            cout << "Enter Appointment Fee: ";
            cin >> appointmentFee;
            if (appointmentFee <= 0) {
                cout << "Appointment fee must be greater than 0. Please try again.\n";
            }
        } while (appointmentFee <= 0);
        cin.ignore();

        // Escape special characters
        char* escapedDescription = new char[description.length() * 2 + 1];
        mysql_real_escape_string(conn, escapedDescription, description.c_str(), description.length());

        query = "INSERT INTO HospitalDepartments (hospital_id, department_id, department_name, department_description, appointment_fee) VALUES (" +
            to_string(hospitalId) + ", " + to_string(departmentId) + ", '" + departmentName + "', '" + escapedDescription + "', " + to_string(appointmentFee) + ")";
        
        delete[] escapedDescription;
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error adding department: " << mysql_error(conn) << "\n";
            return false;
        }

        // Show available surgeries for this department
        cout << "\nAvailable Surgeries for " << departmentName << ":\n";
        query = "SELECT surgery_id, name FROM Surgeries WHERE department_id = " + to_string(departmentId);
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching surgeries: " << mysql_error(conn) << "\n";
            return false;
        }
        
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res))) {
            cout << row[0] << ". " << row[1] << endl;
        }
        mysql_free_result(res);

        // Get surgery fees
        cout << "\nEnter number of surgeries to set fees for: ";
        int numSurgeries;
        cin >> numSurgeries;
        cin.ignore();

        for (int j = 0; j < numSurgeries; j++) {
            int surgeryId;
            double surgeryFee;
            
            cout << "\nSurgery " << (j + 1) << ":\n";
            
            // Validate surgery ID
            do {
                cout << "Enter Surgery ID: ";
                cin >> surgeryId;
                cin.ignore();
                
                // Check if surgery exists and belongs to department
                query = "SELECT COUNT(*) FROM Surgeries WHERE surgery_id = " + to_string(surgeryId) + 
                       " AND department_id = " + to_string(departmentId);
                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error checking surgery: " << mysql_error(conn) << "\n";
                    return false;
                }
                
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);
                if (stoi(row[0]) == 0) {
                    cout << "Invalid surgery ID or surgery does not belong to this department. Please try again.\n";
                }
                mysql_free_result(res);
            } while (stoi(row[0]) == 0);
            
            // Validate surgery fee
            do {
                cout << "Enter Hospital Surgery Fee: ";
                cin >> surgeryFee;
                if (surgeryFee <= 0) {
                    cout << "Surgery fee must be greater than 0. Please try again.\n";
                }
            } while (surgeryFee <= 0);
            cin.ignore();

            query = "INSERT INTO HospitalSurgeryFees (hospital_id, surgery_id, fee) VALUES (" +
                to_string(hospitalId) + ", " + to_string(surgeryId) + ", " + to_string(surgeryFee) + ")";
            
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error adding surgery fee: " << mysql_error(conn) << "\n";
                return false;
            }
        }

        // Handle doctors for this department
        cout << "\nEnter number of registered doctors to add: ";
        int numDoctors;
        cin >> numDoctors;
        cin.ignore();

        for (int k = 0; k < numDoctors; k++) {
            int doctorId;
            
            cout << "\nDoctor " << (k + 1) << ":\n";
            
            // Validate doctor ID
            do {
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                cin.ignore();

                // Verify doctor exists and belongs to the department
                query = "SELECT COUNT(*) FROM Doctors WHERE doctor_id = " + to_string(doctorId) + 
                       " AND department_id = " + to_string(departmentId);
                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error checking doctor: " << mysql_error(conn) << "\n";
                    return false;
                }
                
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);
                if (stoi(row[0]) == 0) {
                    cout << "Invalid doctor ID or doctor does not belong to this department. Please try again.\n";
                }
                mysql_free_result(res);
            } while (stoi(row[0]) == 0);

            query = "INSERT INTO HospitalDoctors (hospital_id, doctor_id, department_id) VALUES (" +
                to_string(hospitalId) + ", " + to_string(doctorId) + ", " + to_string(departmentId) + ")";
            
            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error adding doctor: " << mysql_error(conn) << "\n";
                return false;
            }
        }
    }

    // Handle medical services
    cout << "\n=== Medical Services Setup ===\n";

    // Tests
    cout << "\nEnter number of medical tests: ";
    int numTests;
    cin >> numTests;
    cin.ignore();

    for (int i = 0; i < numTests; i++) {
        string testName;
        double price;
        
        cout << "\nTest " << (i + 1) << ":\n";
        
        // Validate test name
        do {
            cout << "Enter Test Name: ";
            getline(cin, testName);
            if (testName.empty()) {
                cout << "Test name cannot be empty. Please try again.\n";
            }
        } while (testName.empty());
        
        // Validate price
        do {
            cout << "Enter Price: ";
            cin >> price;
            if (price <= 0) {
                cout << "Price must be greater than 0. Please try again.\n";
            }
        } while (price <= 0);
        cin.ignore();

        // Escape special characters
        char* escapedTestName = new char[testName.length() * 2 + 1];
        mysql_real_escape_string(conn, escapedTestName, testName.c_str(), testName.length());

        query = "INSERT INTO Tests (hospital_id, name, price) VALUES (" +
            to_string(hospitalId) + ", '" + escapedTestName + "', " + to_string(price) + ")";
        
        delete[] escapedTestName;
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error adding test: " << mysql_error(conn) << "\n";
            return false;
        }
    }

    // Medicines
    cout << "\nEnter number of medicines: ";
    int numMedicines;
    cin >> numMedicines;
    cin.ignore();

    for (int i = 0; i < numMedicines; i++) {
        string medicineName;
        double price;
        
        cout << "\nMedicine " << (i + 1) << ":\n";
        
        // Validate medicine name
        do {
            cout << "Enter Medicine Name: ";
            getline(cin, medicineName);
            if (medicineName.empty()) {
                cout << "Medicine name cannot be empty. Please try again.\n";
            }
        } while (medicineName.empty());
        
        // Validate price
        do {
            cout << "Enter Price: ";
            cin >> price;
            if (price <= 0) {
                cout << "Price must be greater than 0. Please try again.\n";
            }
        } while (price <= 0);
        cin.ignore();

        // Escape special characters
        char* escapedMedicineName = new char[medicineName.length() * 2 + 1];
        mysql_real_escape_string(conn, escapedMedicineName, medicineName.c_str(), medicineName.length());

        query = "INSERT INTO Medicines (hospital_id, name, price) VALUES (" +
            to_string(hospitalId) + ", '" + escapedMedicineName + "', " + to_string(price) + ")";
        
        delete[] escapedMedicineName;
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error adding medicine: " << mysql_error(conn) << "\n";
            return false;
        }
    }
    
    cout << "\nHospital registration completed successfully!\n";
    return true;
}

bool User::registerUser() 
{
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
        password = getSecurePassword();
        
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
        if (age < 18 && roleChoice != 1) 
        {
            cout << "You must be at least 18 years old to register as a doctor or hospital admin.\n";
            return false;
        }
        
        switch (roleChoice) 
        {
            case 1: role = "patient"; break;
            case 2: role = "doctor"; break;
            case 3: role = "hospital"; break;
            default:
                cout << "Invalid role selection!" << endl;
                return false;
        }

        MYSQL* conn = Database::getConnection();
        if (!conn) 
        {
            throw runtime_error("Database connection failed");
        }

        // Check if email already exists
        string query = "SELECT COUNT(*) FROM Users WHERE email = '" + email + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error checking email: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row && stoi(row[0]) > 0) 
        {
            cout << "Email already registered!" << endl;
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);
        
        // Check if phone already exists
        query = "SELECT COUNT(*) FROM Users WHERE phone = '" + phone + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error checking phone: " << mysql_error(conn) << endl;
            return false;
        }
        
        result = mysql_store_result(conn);
        row = mysql_fetch_row(result);
        if (row && stoi(row[0]) > 0) {
            cout << "Phone number already registered!" << endl;
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);

        // Hash password before inserting
        string hashedPassword = hashPassword(password);
        cout << "Debug: Generated hash for password: " << hashedPassword << endl;
        
        // Insert new user with hashed password
        query = "INSERT INTO Users (name, email, password, phone, dob, role) VALUES ('" +
                name + "', '" + email + "', '" + hashedPassword + "', '" + phone + "', '" + dob + "', '" + role + "')";
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error inserting user: " << mysql_error(conn) << endl;
            return false;
        }

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

        if (!success) 
        {
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

bool User::login(const string& email, const string& password) {
    try {
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cout << "Database connection failed!" << endl;
            return false;
        }

        // First check if user exists
        string query = "SELECT COUNT(*) FROM Users WHERE email = '" + email + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error checking user: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (!row || stoi(row[0]) == 0) {
            cout << "User not found!" << endl;
            mysql_free_result(result);
            return false;
        }
        mysql_free_result(result);

        // Hash the provided password
        string hashedPassword = hashPassword(password);
        
        // Get user details
        query = "SELECT user_id, name, email, phone, dob, role, password FROM Users WHERE email = '" + email + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching user details: " << mysql_error(conn) << endl;
            return false;
        }

        result = mysql_store_result(conn);
        if (!result || mysql_num_rows(result) == 0) {
            cout << "User not found!" << endl;
            if (result) mysql_free_result(result);
            return false;
        }

        row = mysql_fetch_row(result);
        if (!row) {
            cout << "Error fetching user data!" << endl;
            mysql_free_result(result);
            return false;
        }

        // Compare hashed passwords
        if (hashedPassword != row[6]) {
            cout << "Invalid password! (Debug: Provided hash=" << hashedPassword << ", Stored hash=" << row[6] << ")" << endl;
            mysql_free_result(result);
            return false;
        }

        // Set user details
        user_id = stoi(row[0]);
        name = row[1];
        this->email = row[2];
        phone = row[3];
        dob = row[4];
        role = row[5];

        mysql_free_result(result);
        cout << "Login successful!" << endl;
        return true;

    } catch (const exception& e) {
        cout << "Login error: " << e.what() << endl;
        return false;
    }
}

bool User::logout() {
    user_id = 0;
    name.clear();
    email.clear();
    phone.clear();
    dob.clear();
    role.clear();
    return true;
}

string User::hashPassword(const string& password) {
    // Use a simple but consistent hashing method
    string salt = "CareSphere2024";
    string saltedPassword = password + salt;
    size_t hash = 0;
    for (char c : saltedPassword) {
        hash = ((hash << 5) - hash) + c;
        hash = hash & hash; // Convert to 32-bit integer
    }
    return to_string(hash);
}

bool User::validateEmail(const string& email) {
    regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex_match(email, pattern);
}

bool User::validatePhone(const string& phone) {
    regex pattern("^[0-9]{11}$");  // Assuming 11-digit phone numbers
    return regex_match(phone, pattern);
}

bool User::validatePassword(const string& password) {
    // At least 8 characters, 1 uppercase, 1 lowercase, 1 number
    regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).{8,}$");
    return regex_match(password, pattern);
}

bool User::validateName(const string& name) {
    regex pattern("^[a-zA-Z ]{2,50}$");
    return regex_match(name, pattern);
}

bool User::validateDOB(const string& dob) {
    regex pattern("^\\d{4}-\\d{2}-\\d{2}$");
    if (!regex_match(dob, pattern)) return false;
    return true;
} 