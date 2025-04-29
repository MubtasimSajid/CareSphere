#include "../include/Hospital.h"
#include "../include/Database.h"
#include <iostream>
#include <iomanip>
#include <mysql.h>
#include <regex>

using namespace std;

Hospital::Hospital() : User() {
    has_ambulance = false;
    income = 0.0;
    expenditure = 0.0;
    average_rating = 0.0;
    total_ratings = 0;
}

bool Hospital::validatePincode(const string& pincode) {
    return !pincode.empty() && pincode.length() <= 10;
}

bool Hospital::validateAddress(const string& address) {
    return !address.empty();
}

bool Hospital::validatePhone(const string& phone) {
    regex pattern("^[0-9]{11}$");
    return regex_match(phone, pattern);
}

bool Hospital::addDepartment(int departmentId) {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "INSERT INTO HospitalDepartments (hospital_id, department_id) VALUES (" +
                  to_string(user_id) + ", " + to_string(departmentId) + ")";
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error adding department: " << mysql_error(conn) << "\n";
        return false;
    }

    department_ids.push_back(departmentId);
    return true;
}

bool Hospital::removeDepartment(int departmentId) {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "DELETE FROM HospitalDepartments WHERE hospital_id = " + to_string(user_id) +
                  " AND department_id = " + to_string(departmentId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error removing department: " << mysql_error(conn) << "\n";
        return false;
    }

    auto it = find(department_ids.begin(), department_ids.end(), departmentId);
    if (it != department_ids.end()) {
        department_ids.erase(it);
    }
    return true;
}

bool Hospital::addDoctor(int doctorId) {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "INSERT INTO HospitalDoctors (hospital_id, doctor_id) VALUES (" +
                  to_string(user_id) + ", " + to_string(doctorId) + ")";
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error adding doctor: " << mysql_error(conn) << "\n";
        return false;
    }

    doctor_ids.push_back(doctorId);
    return true;
}

bool Hospital::removeDoctor(int doctorId) {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "DELETE FROM HospitalDoctors WHERE hospital_id = " + to_string(user_id) +
                  " AND doctor_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error removing doctor: " << mysql_error(conn) << "\n";
        return false;
    }

    auto it = find(doctor_ids.begin(), doctor_ids.end(), doctorId);
    if (it != doctor_ids.end()) {
        doctor_ids.erase(it);
    }
    return true;
}

bool Hospital::updateRating(double newRating) {
    if (newRating < 0 || newRating > 5) return false;

    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    // Update total ratings and average
    total_ratings++;
    average_rating = ((average_rating * (total_ratings - 1)) + newRating) / total_ratings;

    string query = "UPDATE Hospitals SET average_rating = " + to_string(average_rating) +
                  ", total_ratings = " + to_string(total_ratings) +
                  " WHERE hospital_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error updating rating: " << mysql_error(conn) << "\n";
        return false;
    }

    return true;
}

bool Hospital::updateFinancials(double newIncome, double newExpenditure) {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    income += newIncome;
    expenditure += newExpenditure;

    string query = "UPDATE Hospitals SET income = " + to_string(income) +
                  ", expenditure = " + to_string(expenditure) +
                  " WHERE hospital_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error updating financials: " << mysql_error(conn) << "\n";
        return false;
    }

    return true;
}

void Hospital::displayInfo() const {
    cout << "\n=== Hospital Information ===\n";
    cout << "Name: " << hospital_name << endl;
    cout << "Address: " << address << endl;
    cout << "City: " << city << endl;
    cout << "State: " << state << endl;
    cout << "Country: " << country << endl;
    cout << "Pincode: " << pincode << endl;
    cout << "Ambulance Service: " << (has_ambulance ? "Available" : "Not Available") << endl;
    cout << "Average Rating: " << fixed << setprecision(2) << average_rating << " (" << total_ratings << " ratings)" << endl;
    cout << "Financial Status:" << endl;
    cout << "  Income: $" << fixed << setprecision(2) << income << endl;
    cout << "  Expenditure: $" << fixed << setprecision(2) << expenditure << endl;
    cout << "  Net Balance: $" << fixed << setprecision(2) << (income - expenditure) << endl;
}

bool Hospital::displayDepartments() const {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "SELECT d.department_id, d.name, hd.department_description, hd.appointment_fee "
                  "FROM HospitalDepartments hd "
                  "JOIN Departments d ON hd.department_id = d.department_id "
                  "WHERE hd.hospital_id = " + to_string(user_id);

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching departments: " << mysql_error(conn) << "\n";
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No departments found.\n";
        mysql_free_result(result);
        return false;
    }

    cout << "\n=== Hospital Departments ===\n";
    cout << left << setw(5) << "ID" << setw(30) << "Name" << setw(50) << "Description" 
         << "Appointment Fee" << endl;
    cout << string(95, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(5) << row[0] << setw(30) << row[1] << setw(50) << row[2]
             << "$" << fixed << setprecision(2) << stod(row[3]) << endl;
    }

    mysql_free_result(result);
    return true;
}

bool Hospital::displayDoctors() const {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "SELECT d.doctor_id, u.name, d.specialization, d.appointment_fees, "
                  "d.average_rating, d.total_ratings "
                  "FROM HospitalDoctors hd "
                  "JOIN Doctors d ON hd.doctor_id = d.doctor_id "
                  "JOIN Users u ON d.user_id = u.user_id "
                  "WHERE hd.hospital_id = " + to_string(user_id);

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching doctors: " << mysql_error(conn) << "\n";
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No doctors found.\n";
        mysql_free_result(result);
        return false;
    }

    cout << "\n=== Hospital Doctors ===\n";
    cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(25) << "Specialization"
         << setw(15) << "Fee" << setw(15) << "Rating" << endl;
    cout << string(95, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(5) << row[0] << setw(25) << row[1] << setw(25) << row[2]
             << "$" << fixed << setprecision(2) << stod(row[3]) << setw(15)
             << (row[4] ? row[4] : "No ratings") << endl;
    }

    mysql_free_result(result);
    return true;
}

bool Hospital::displayAmbulances() const {
    if (!has_ambulance) {
        cout << "This hospital does not provide ambulance service.\n";
        return true;
    }

    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    string query = "SELECT vehicle_number, phone_number, fee_per_km "
                  "FROM HospitalAmbulances "
                  "WHERE hospital_id = " + to_string(user_id);

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching ambulances: " << mysql_error(conn) << "\n";
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No ambulances found.\n";
        mysql_free_result(result);
        return false;
    }

    cout << "\n=== Ambulance Service ===\n";
    cout << left << setw(20) << "Vehicle Number" << setw(15) << "Phone Number"
         << "Fee per Kilometer" << endl;
    cout << string(55, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(20) << row[0] << setw(15) << row[1]
             << "$" << fixed << setprecision(2) << stod(row[2]) << endl;
    }

    mysql_free_result(result);
    return true;
}

bool Hospital::displayMedicalServices() const {
    MYSQL* conn = Database::getConnection();
    if (!conn) return false;

    // Display tests
    cout << "\n=== Medical Tests ===\n";
    string query = "SELECT name, price FROM Tests WHERE hospital_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching tests: " << mysql_error(conn) << "\n";
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result && mysql_num_rows(result) > 0) {
        cout << left << setw(40) << "Test Name" << "Price" << endl;
        cout << string(50, '-') << endl;

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << left << setw(40) << row[0] 
                 << "$" << fixed << setprecision(2) << stod(row[1]) << endl;
        }
    } else {
        cout << "No tests available.\n";
    }
    mysql_free_result(result);

    // Display medicines
    cout << "\n=== Medicines ===\n";
    query = "SELECT name, price FROM Medicines WHERE hospital_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching medicines: " << mysql_error(conn) << "\n";
        return false;
    }

    result = mysql_store_result(conn);
    if (result && mysql_num_rows(result) > 0) {
        cout << left << setw(40) << "Medicine Name" << "Price" << endl;
        cout << string(50, '-') << endl;

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            cout << left << setw(40) << row[0] 
                 << "$" << fixed << setprecision(2) << stod(row[1]) << endl;
        }
    } else {
        cout << "No medicines available.\n";
    }
    mysql_free_result(result);

    return true;
}

bool Hospital::registerUser() {
    try {
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cout << "Database connection failed!" << endl;
            return false;
        }

        string name, email, phone, password, dob;
        string hospitalName, address, city, state, country, pincode;
        bool hasAmbulance;

        cout << "\n=== Hospital Registration ===\n";
        
        // Get basic user information
        cout << "Enter name: ";
        getline(cin, name);
        if (!validateName(name)) return false;

        cout << "Enter email: ";
        getline(cin, email);
        if (!validateEmail(email)) return false;

        cout << "Enter password: ";
        password = getSecurePassword();
        if (!validatePassword(password)) return false;

        cout << "Enter phone number (11 digits): ";
        getline(cin, phone);
        if (!validatePhone(phone)) return false;

        cout << "Enter date of birth (YYYY-MM-DD): ";
        getline(cin, dob);
        if (!validateDOB(dob)) return false;

        // Get hospital-specific information
        cout << "Enter hospital name: ";
        getline(cin, hospitalName);
        if (hospitalName.empty()) {
            cout << "Hospital name cannot be empty.\n";
            return false;
        }

        cout << "Enter address: ";
        getline(cin, address);
        if (!validateAddress(address)) return false;

        cout << "Enter city: ";
        getline(cin, city);
        if (city.empty()) {
            cout << "City cannot be empty.\n";
            return false;
        }

        cout << "Enter state: ";
        getline(cin, state);
        if (state.empty()) {
            cout << "State cannot be empty.\n";
            return false;
        }

        cout << "Enter country: ";
        getline(cin, country);
        if (country.empty()) {
            cout << "Country cannot be empty.\n";
            return false;
        }

        cout << "Enter pincode: ";
        getline(cin, pincode);
        if (!validatePincode(pincode)) return false;

        cout << "Does the hospital have ambulance service? (1 for yes, 0 for no): ";
        cin >> hasAmbulance;
        cin.ignore();

        // Start transaction
        mysql_query(conn, "START TRANSACTION");

        // Escape special characters
        char escapedName[256], escapedEmail[256], escapedPhone[256], 
             escapedPassword[256], escapedDob[256], escapedHospitalName[256],
             escapedAddress[256], escapedCity[256], escapedState[256], 
             escapedCountry[256], escapedPincode[256];
        
        mysql_real_escape_string(conn, escapedName, name.c_str(), name.length());
        mysql_real_escape_string(conn, escapedEmail, email.c_str(), email.length());
        mysql_real_escape_string(conn, escapedPhone, phone.c_str(), phone.length());
        mysql_real_escape_string(conn, escapedPassword, password.c_str(), password.length());
        mysql_real_escape_string(conn, escapedDob, dob.c_str(), dob.length());
        mysql_real_escape_string(conn, escapedHospitalName, hospitalName.c_str(), hospitalName.length());
        mysql_real_escape_string(conn, escapedAddress, address.c_str(), address.length());
        mysql_real_escape_string(conn, escapedCity, city.c_str(), city.length());
        mysql_real_escape_string(conn, escapedState, state.c_str(), state.length());
        mysql_real_escape_string(conn, escapedCountry, country.c_str(), country.length());
        mysql_real_escape_string(conn, escapedPincode, pincode.c_str(), pincode.length());

        // Insert into Users table
        string query = "INSERT INTO Users (name, email, password, phone, dob, role) VALUES ('" +
                      string(escapedName) + "', '" + string(escapedEmail) + "', '" +
                      string(escapedPassword) + "', '" + string(escapedPhone) + "', '" +
                      string(escapedDob) + "', 'hospital')";

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error inserting user: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }

        user_id = mysql_insert_id(conn);

        // Insert into Hospitals table
        query = "INSERT INTO Hospitals (user_id, hospital_name, address, city, state, country, pincode, has_ambulance) VALUES (" +
                to_string(user_id) + ", '" + string(escapedHospitalName) + "', '" +
                string(escapedAddress) + "', '" + string(escapedCity) + "', '" +
                string(escapedState) + "', '" + string(escapedCountry) + "', '" +
                string(escapedPincode) + "', " + to_string(hasAmbulance) + ")";

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error inserting hospital: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }

        // Get the hospital_id from the Hospitals table
        query = "SELECT hospital_id FROM Hospitals WHERE user_id = " + to_string(user_id);
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error getting hospital_id: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        int hospital_id = atoi(row[0]);
        mysql_free_result(result);

        // If ambulance service is available, add ambulance details
        if (hasAmbulance) {
            cout << "\n=== Add Ambulance Service ===\n";
            int numAmbulances;
            cout << "How many ambulances do you want to add? ";
            cin >> numAmbulances;
            cin.ignore();

            for (int i = 0; i < numAmbulances; i++) {
                string vehicleNumber;
                cout << "\nEnter Vehicle Number: ";
                getline(cin, vehicleNumber);

                string phoneNumber;
                cout << "Enter Phone Number: ";
                getline(cin, phoneNumber);

                double feePerKm;
                cout << "Enter Fee per Kilometer: $";
                cin >> feePerKm;
                cin.ignore();

                char* escapedVehicle = new char[vehicleNumber.length() * 2 + 1];
                char* escapedPhone = new char[phoneNumber.length() * 2 + 1];
                mysql_real_escape_string(conn, escapedVehicle, vehicleNumber.c_str(), vehicleNumber.length());
                mysql_real_escape_string(conn, escapedPhone, phoneNumber.c_str(), phoneNumber.length());

                query = "INSERT INTO HospitalAmbulances (hospital_id, vehicle_number, phone_number, fee_per_km) VALUES (" +
                        to_string(hospital_id) + ", '" + escapedVehicle + "', '" + escapedPhone + "', " + to_string(feePerKm) + ")";

                delete[] escapedVehicle;
                delete[] escapedPhone;

                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error adding ambulance: " << mysql_error(conn) << endl;
                    mysql_query(conn, "ROLLBACK");
                    return false;
                }
            }
        }

        // Add departments
        cout << "\n=== Add Departments ===\n";
        cout << "Available Departments:\n";
        query = "SELECT department_id, name FROM Departments";
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching departments: " << mysql_error(conn) << endl;
            mysql_query(conn, "ROLLBACK");
            return false;
        }

        result = mysql_store_result(conn);
        while ((row = mysql_fetch_row(result))) {
            cout << row[0] << ". " << row[1] << endl;
        }
        mysql_free_result(result);

        int numDepartments;
        cout << "\nHow many departments do you want to add? ";
        cin >> numDepartments;
        cin.ignore();

        for (int i = 0; i < numDepartments; i++) {
            int departmentId;
            cout << "\nEnter Department ID: ";
            cin >> departmentId;
            cin.ignore();

            string departmentName;
            cout << "Enter Department Name: ";
            getline(cin, departmentName);

            string description;
            cout << "Enter Department Description: ";
            getline(cin, description);

            double appointmentFee;
            cout << "Enter Appointment Fee: $";
            cin >> appointmentFee;
            cin.ignore();

            // Escape special characters
            char* escapedDeptName = new char[departmentName.length() * 2 + 1];
            char* escapedDesc = new char[description.length() * 2 + 1];
            mysql_real_escape_string(conn, escapedDeptName, departmentName.c_str(), departmentName.length());
            mysql_real_escape_string(conn, escapedDesc, description.c_str(), description.length());

            query = "INSERT INTO HospitalDepartments (hospital_id, department_id, department_name, department_description, appointment_fee) "
                    "VALUES (" + to_string(hospital_id) + ", " + to_string(departmentId) + ", '" + 
                    escapedDeptName + "', '" + escapedDesc + "', " + to_string(appointmentFee) + ")";

            delete[] escapedDeptName;
            delete[] escapedDesc;

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error adding department: " << mysql_error(conn) << endl;
                mysql_query(conn, "ROLLBACK");
                return false;
            }

            // Add doctors for this department
            cout << "\n=== Add Doctors for " << departmentName << " ===\n";
            int numDoctors;
            cout << "How many doctors do you want to add for this department? ";
            cin >> numDoctors;
            cin.ignore();

            for (int j = 0; j < numDoctors; j++) {
                string doctorName, doctorEmail, doctorPhone, doctorPassword, doctorDob, specialization;
                double doctorFee;

                cout << "\nEnter Doctor Name: ";
                getline(cin, doctorName);

                cout << "Enter Doctor Email: ";
                getline(cin, doctorEmail);

                cout << "Enter Doctor Password: ";
                doctorPassword = getSecurePassword();

                cout << "Enter Doctor Phone: ";
                getline(cin, doctorPhone);

                cout << "Enter Doctor Date of Birth (YYYY-MM-DD): ";
                getline(cin, doctorDob);

                cout << "Enter Specialization: ";
                getline(cin, specialization);

                cout << "Enter Appointment Fee: $";
                cin >> doctorFee;
                cin.ignore();

                // Escape special characters
                char* escapedDoctorName = new char[doctorName.length() * 2 + 1];
                char* escapedDoctorEmail = new char[doctorEmail.length() * 2 + 1];
                char* escapedDoctorPhone = new char[doctorPhone.length() * 2 + 1];
                char* escapedDoctorDob = new char[doctorDob.length() * 2 + 1];
                char* escapedSpecialization = new char[specialization.length() * 2 + 1];
                char* escapedDoctorPassword = new char[doctorPassword.length() * 2 + 1];

                mysql_real_escape_string(conn, escapedDoctorName, doctorName.c_str(), doctorName.length());
                mysql_real_escape_string(conn, escapedDoctorEmail, doctorEmail.c_str(), doctorEmail.length());
                mysql_real_escape_string(conn, escapedDoctorPhone, doctorPhone.c_str(), doctorPhone.length());
                mysql_real_escape_string(conn, escapedDoctorDob, doctorDob.c_str(), doctorDob.length());
                mysql_real_escape_string(conn, escapedSpecialization, specialization.c_str(), specialization.length());
                mysql_real_escape_string(conn, escapedDoctorPassword, doctorPassword.c_str(), doctorPassword.length());

                // Insert into Users table
                query = "INSERT INTO Users (name, email, password, phone, dob, role) VALUES ('" +
                        string(escapedDoctorName) + "', '" + string(escapedDoctorEmail) + "', '" +
                        string(escapedDoctorPassword) + "', '" + string(escapedDoctorPhone) + "', '" +
                        string(escapedDoctorDob) + "', 'doctor')";

                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error inserting doctor user: " << mysql_error(conn) << endl;
                    mysql_query(conn, "ROLLBACK");
                    delete[] escapedDoctorName;
                    delete[] escapedDoctorEmail;
                    delete[] escapedDoctorPhone;
                    delete[] escapedDoctorDob;
                    delete[] escapedSpecialization;
                    delete[] escapedDoctorPassword;
                    return false;
                }

                int doctorUserId = mysql_insert_id(conn);

                // Insert into Doctors table
                query = "INSERT INTO Doctors (user_id, specialization, appointment_fees) VALUES (" +
                        to_string(doctorUserId) + ", '" + string(escapedSpecialization) + "', " +
                        to_string(doctorFee) + ")";

                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error inserting doctor: " << mysql_error(conn) << endl;
                    mysql_query(conn, "ROLLBACK");
                    delete[] escapedDoctorName;
                    delete[] escapedDoctorEmail;
                    delete[] escapedDoctorPhone;
                    delete[] escapedDoctorDob;
                    delete[] escapedSpecialization;
                    delete[] escapedDoctorPassword;
                    return false;
                }

                int doctorId = mysql_insert_id(conn);

                // Associate doctor with hospital and department
                query = "INSERT INTO HospitalDoctors (hospital_id, doctor_id, department_id) VALUES (" +
                        to_string(hospital_id) + ", " + to_string(doctorId) + ", " + to_string(departmentId) + ")";

                if (mysql_query(conn, query.c_str()) != 0) {
                    cout << "Error associating doctor with department: " << mysql_error(conn) << endl;
                    mysql_query(conn, "ROLLBACK");
                    delete[] escapedDoctorName;
                    delete[] escapedDoctorEmail;
                    delete[] escapedDoctorPhone;
                    delete[] escapedDoctorDob;
                    delete[] escapedSpecialization;
                    delete[] escapedDoctorPassword;
                    return false;
                }

                delete[] escapedDoctorName;
                delete[] escapedDoctorEmail;
                delete[] escapedDoctorPhone;
                delete[] escapedDoctorDob;
                delete[] escapedSpecialization;
                delete[] escapedDoctorPassword;
            }
        }

        // Add medical services
        cout << "\n=== Add Medical Services ===\n";
        int numTests, numMedicines;
        cout << "How many medical tests do you want to add? ";
        cin >> numTests;
        cin.ignore();

        for (int i = 0; i < numTests; i++) {
            string testName;
            cout << "\nEnter Test Name: ";
            getline(cin, testName);

            double price;
            cout << "Enter Test Price: $";
            cin >> price;
            cin.ignore();

            char* escapedTestName = new char[testName.length() * 2 + 1];
            mysql_real_escape_string(conn, escapedTestName, testName.c_str(), testName.length());

            query = "INSERT INTO Tests (hospital_id, name, price) VALUES (" +
                    to_string(hospital_id) + ", '" + escapedTestName + "', " + to_string(price) + ")";

            delete[] escapedTestName;

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error adding test: " << mysql_error(conn) << endl;
                mysql_query(conn, "ROLLBACK");
                return false;
            }
        }

        cout << "How many medicines do you want to add? ";
        cin >> numMedicines;
        cin.ignore();

        for (int i = 0; i < numMedicines; i++) {
            string medicineName;
            cout << "\nEnter Medicine Name: ";
            getline(cin, medicineName);

            double price;
            cout << "Enter Medicine Price: $";
            cin >> price;
            cin.ignore();

            char* escapedMedicineName = new char[medicineName.length() * 2 + 1];
            mysql_real_escape_string(conn, escapedMedicineName, medicineName.c_str(), medicineName.length());

            query = "INSERT INTO Medicines (hospital_id, name, price) VALUES (" +
                    to_string(hospital_id) + ", '" + escapedMedicineName + "', " + to_string(price) + ")";

            delete[] escapedMedicineName;

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error adding medicine: " << mysql_error(conn) << endl;
                mysql_query(conn, "ROLLBACK");
                return false;
            }
        }

        // Commit transaction
        mysql_query(conn, "COMMIT");

        // Update local object
        this->name = name;
        this->email = email;
        this->phone = phone;
        this->dob = dob;
        this->role = "hospital";
        this->hospital_name = hospitalName;
        this->address = address;
        this->city = city;
        this->state = state;
        this->country = country;
        this->pincode = pincode;
        this->has_ambulance = hasAmbulance;

        cout << "\nHospital registered successfully!\n";
        return true;

    } catch (const exception& e) {
        cout << "Registration error: " << e.what() << endl;
        return false;
    }
}

bool Hospital::login(const string& email, const string& password) {
    try {
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cout << "Database connection failed!" << endl;
            return false;
        }

        // First check if user exists and is a hospital
        string query = "SELECT u.user_id, u.name, u.email, u.phone, u.dob, u.role, "
                      "h.hospital_name, h.address, h.city, h.state, h.country, h.pincode, "
                      "h.has_ambulance, h.income, h.expenditure, h.average_rating, h.total_ratings "
                      "FROM Users u "
                      "JOIN Hospitals h ON u.user_id = h.user_id "
                      "WHERE u.email = '" + email + "' AND u.role = 'hospital'";
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error checking user: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (!result || mysql_num_rows(result) == 0) {
            cout << "Hospital not found!" << endl;
            if (result) mysql_free_result(result);
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (!row) {
            cout << "Error fetching user data!" << endl;
            mysql_free_result(result);
            return false;
        }

        // Update local object
        user_id = stoi(row[0]);
        this->name = row[1];
        this->email = row[2];
        this->phone = row[3];
        this->dob = row[4];
        this->role = row[5];
        this->hospital_name = row[6];
        this->address = row[7];
        this->city = row[8];
        this->state = row[9];
        this->country = row[10];
        this->pincode = row[11];
        this->has_ambulance = (row[12][0] == '1');
        this->income = stod(row[13]);
        this->expenditure = stod(row[14]);
        this->average_rating = row[15] ? stod(row[15]) : 0.0;
        this->total_ratings = row[16] ? stoi(row[16]) : 0;

        mysql_free_result(result);
        cout << "Login successful!" << endl;
        return true;

    } catch (const exception& e) {
        cout << "Login error: " << e.what() << endl;
        return false;
    }
}

bool Hospital::logout() {
    try {
        // Clear all hospital-specific data
        hospital_name.clear();
        address.clear();
        city.clear();
        state.clear();
        country.clear();
        pincode.clear();
        has_ambulance = false;
        income = 0.0;
        expenditure = 0.0;
        average_rating = 0.0;
        total_ratings = 0;
        department_ids.clear();
        doctor_ids.clear();

        // Call base class logout to clear user data
        return User::logout();
    } catch (const exception& e) {
        cout << "Logout error: " << e.what() << endl;
        return false;
    }
}

bool Hospital::updateProfile() {
    try {
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cout << "Database connection failed!" << endl;
            return false;
        }

        string name, phone, address, city, state, country, pincode;
        bool hasAmbulance;

        cout << "\n=== Update Hospital Profile ===\n";
        
        // Get new values with validation
        do {
            cout << "Enter new hospital name: ";
            getline(cin, name);
            if (name.empty()) {
                cout << "Hospital name cannot be empty.\n";
            }
        } while (name.empty());

        do {
            cout << "Enter new phone number (11 digits): ";
            getline(cin, phone);
            if (!validatePhone(phone)) {
                cout << "Invalid phone number format.\n";
            }
        } while (!validatePhone(phone));

        do {
            cout << "Enter new address: ";
            getline(cin, address);
            if (!validateAddress(address)) {
                cout << "Invalid address format.\n";
            }
        } while (!validateAddress(address));

        do {
            cout << "Enter new city: ";
            getline(cin, city);
            if (city.empty()) {
                cout << "City cannot be empty.\n";
            }
        } while (city.empty());

        do {
            cout << "Enter new state: ";
            getline(cin, state);
            if (state.empty()) {
                cout << "State cannot be empty.\n";
            }
        } while (state.empty());

        do {
            cout << "Enter new country: ";
            getline(cin, country);
            if (country.empty()) {
                cout << "Country cannot be empty.\n";
            }
        } while (country.empty());

        do {
            cout << "Enter new pincode: ";
            getline(cin, pincode);
            if (!validatePincode(pincode)) {
                cout << "Invalid pincode format.\n";
            }
        } while (!validatePincode(pincode));

        cout << "Does the hospital have ambulance service? (1 for yes, 0 for no): ";
        cin >> hasAmbulance;
        cin.ignore();

        // Escape special characters
        char escapedName[256], escapedPhone[256], escapedAddress[256], 
             escapedCity[256], escapedState[256], escapedCountry[256], 
             escapedPincode[256];
        
        mysql_real_escape_string(conn, escapedName, name.c_str(), name.length());
        mysql_real_escape_string(conn, escapedPhone, phone.c_str(), phone.length());
        mysql_real_escape_string(conn, escapedAddress, address.c_str(), address.length());
        mysql_real_escape_string(conn, escapedCity, city.c_str(), city.length());
        mysql_real_escape_string(conn, escapedState, state.c_str(), state.length());
        mysql_real_escape_string(conn, escapedCountry, country.c_str(), country.length());
        mysql_real_escape_string(conn, escapedPincode, pincode.c_str(), pincode.length());

        // Update user information
        string query = "UPDATE Users SET name = '" + string(escapedName) + 
                      "', phone = '" + string(escapedPhone) + 
                      "' WHERE user_id = " + to_string(user_id);

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error updating user information: " << mysql_error(conn) << endl;
            return false;
        }

        // Update hospital information
        query = "UPDATE Hospitals SET hospital_name = '" + string(escapedName) + 
                "', address = '" + string(escapedAddress) + 
                "', city = '" + string(escapedCity) + 
                "', state = '" + string(escapedState) + 
                "', country = '" + string(escapedCountry) + 
                "', pincode = '" + string(escapedPincode) + 
                "', has_ambulance = " + to_string(hasAmbulance) + 
                " WHERE user_id = " + to_string(user_id);

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error updating hospital information: " << mysql_error(conn) << endl;
            return false;
        }

        // Update local object
        hospital_name = name;
        this->phone = phone;
        this->address = address;
        this->city = city;
        this->state = state;
        this->country = country;
        this->pincode = pincode;
        this->has_ambulance = hasAmbulance;

        cout << "\nProfile updated successfully!\n";
        return true;

    } catch (const exception& e) {
        cout << "Error updating profile: " << e.what() << endl;
        return false;
    }
}

bool Hospital::loadHospitalData(int userId) {
    try {
        MYSQL* conn = Database::getConnection();
        if (!conn) {
            cout << "Database connection failed!" << endl;
            return false;
        }

        string query = "SELECT u.user_id, u.name, u.email, u.phone, u.dob, u.role, "
                      "h.hospital_name, h.address, h.city, h.state, h.country, h.pincode, "
                      "h.has_ambulance, h.income, h.expenditure, h.average_rating, h.total_ratings "
                      "FROM Users u "
                      "JOIN Hospitals h ON u.user_id = h.user_id "
                      "WHERE u.user_id = " + to_string(userId);
        
        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error checking user: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (!result || mysql_num_rows(result) == 0) {
            cout << "Hospital not found!" << endl;
            if (result) mysql_free_result(result);
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (!row) {
            cout << "Error fetching user data!" << endl;
            mysql_free_result(result);
            return false;
        }

        // Update local object
        user_id = stoi(row[0]);
        this->name = row[1];
        this->email = row[2];
        this->phone = row[3];
        this->dob = row[4];
        this->role = row[5];
        this->hospital_name = row[6];
        this->address = row[7];
        this->city = row[8];
        this->state = row[9];
        this->country = row[10];
        this->pincode = row[11];
        this->has_ambulance = (row[12][0] == '1');
        this->income = stod(row[13]);
        this->expenditure = stod(row[14]);
        this->average_rating = row[15] ? stod(row[15]) : 0.0;
        this->total_ratings = row[16] ? stoi(row[16]) : 0;

        mysql_free_result(result);
        return true;

    } catch (const exception& e) {
        cout << "Error loading hospital data: " << e.what() << endl;
        return false;
    }
} 