#define _CRT_SECURE_NO_WARNINGS
#include "../include/DoctorDashboard.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mysql.h>
#include <ctime>
#include <algorithm>
#include <limits>

DoctorDashboard::DoctorDashboard(int userId) {
    doctor = new Doctor();
    loadDoctorData(userId);
}

void DoctorDashboard::loadDoctorData(int userId) {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT u.user_id, u.name, u.email, u.phone, u.dob, "
                  "d.department_id, d.specialization, d.appointment_fees "
                  "FROM Users u "
                  "JOIN Doctors d ON u.user_id = d.user_id "
                  "WHERE u.user_id = " + to_string(userId);

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            // Set base class (User) data
            doctor->user_id = stoi(row[0]);
            doctor->name = row[1];
            doctor->email = row[2];
            doctor->phone = row[3];
            doctor->dob = row[4];
            
            // Set Doctor-specific data with NULL checks
            if (row[6]) doctor->setSpecialization(row[6]);
            if (row[7]) doctor->setAppointmentFees(stod(row[7]));
        } else {
            cout << "Error: Doctor data not found for user ID: " << userId << endl;
            throw runtime_error("Doctor data not found");
        }
        mysql_free_result(result);
    } else {
        cout << "Error loading doctor data: " << mysql_error(conn) << endl;
        throw runtime_error("Failed to load doctor data");
    }

    // Load degrees
    query = "SELECT degree_name FROM DoctorDegrees WHERE doctor_id = " + to_string(userId);
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            doctor->addDegree(row[0]);
        }
        mysql_free_result(result);
    }

    // Load hospital associations
    query = "SELECT hospital_id FROM HospitalDoctors WHERE doctor_id = " + to_string(userId);
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            doctor->addHospital(stoi(row[0]));
        }
        mysql_free_result(result);
    }
}

void DoctorDashboard::displayDashboard() {
    int choice;
    do {
        cout << "\n=================================\n";
        cout << "     Doctor Dashboard Menu\n";
        cout << "=================================\n";
        cout << "1. View Profile\n";
        cout << "2. Make a Note\n";
        cout << "3. View Notes\n";
        cout << "4. View Schedule\n";
        cout << "5. View Patients\n";
        cout << "6. Prescribe Medication\n";
        cout << "7. Show Balance\n";
        cout << "8. Show Balance History\n";
        cout << "9. Withdraw Money\n";
        cout << "10. Update Profile\n";
        cout << "11. Logout\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                showProfile();
                break;
            case 2:
                makeNote();
                break;
            case 3:
                showNotes();
                break;
            case 4:
                showSchedule();
                break;
            case 5:
                viewPatients();
                break;
            case 6:
                prescribeMedication();
                break;
            case 7:
                showBalance();
                break;
            case 8:
                showBalanceHistory();
                break;
            case 9:
                withdrawMoney();
                break;
            case 10:
                updateProfile();
                break;
            case 11:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 11);
}

void DoctorDashboard::showProfile() const {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT u.name, u.email, u.phone, u.dob, "
                  "d.department_id, d.specialization, d.appointment_fees, d.license_number, "
                  "GROUP_CONCAT(DISTINCT ds.degree_name) as degrees "
                  "FROM Users u "
                  "JOIN Doctors d ON u.user_id = d.user_id "
                  "LEFT JOIN DoctorDegrees ds ON d.doctor_id = ds.doctor_id "
                  "WHERE u.user_id = " + to_string(doctor->getUserId()) + " "
                  "GROUP BY u.name, u.email, u.phone, u.dob, "
                  "d.department_id, d.specialization, d.appointment_fees, d.license_number";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "Error fetching profile: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        cout << "\n=== Doctor Profile ===\n";
        cout << "Name: " << row[0] << endl;
        cout << "Email: " << row[1] << endl;
        cout << "Phone: " << row[2] << endl;
        cout << "Date of Birth: " << row[3] << endl;
        cout << "Department: " << getDepartmentName(stoi(string(row[4]))) << endl;
        cout << "Specialization: " << row[5] << endl;
        cout << "Appointment Fee: " << row[6] << " BDT" << endl;
        cout << "License Number: " << row[7] << endl;
        cout << "Degrees: " << (row[8] ? row[8] : "None") << endl;
        cout << "===================\n";
    }

    mysql_free_result(result);
}

void DoctorDashboard::makeNote() {
    cout << "\nEnter your note: ";
    string note;
    getline(cin, note);

    MYSQL* conn = Database::getConnection();
    
    // First get the doctor_id from the Doctors table
    string query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found in database\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorId = stoi(row[0]);
    mysql_free_result(result);
    
    // Escape special characters in the note
    char* escapedNote = new char[note.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedNote, note.c_str(), note.length());
    
    // Insert the note using the correct doctor_id
    query = "INSERT INTO DoctorNotes (doctor_id, note, created_at) VALUES (" +
            to_string(doctorId) + ", '" + escapedNote + "', NOW())";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Note added successfully!\n";
    } else {
        cout << "Error adding note: " << mysql_error(conn) << "\n";
    }
    
    delete[] escapedNote;
}

void DoctorDashboard::showNotes() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the doctor_id from the Doctors table
    string query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found in database\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now fetch the notes using the correct doctor_id
    query = "SELECT note, created_at FROM DoctorNotes "
            "WHERE doctor_id = " + to_string(doctorId) +
            " ORDER BY created_at DESC";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Your Notes ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No notes found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Date: " << row[1] << endl;
                cout << "Note: " << row[0] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error fetching notes: " << mysql_error(conn) << "\n";
    }
}

void DoctorDashboard::showSchedule() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the doctor_id from the Doctors table
    string query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found in database\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now fetch the schedule using the correct doctor_id
    query = "SELECT a.appointment_id, u.name, a.appointment_date, a.appointment_time, a.status "
            "FROM Appointments a "
            "JOIN Patients p ON a.patient_id = p.patient_id "
            "JOIN Users u ON p.user_id = u.user_id "
            "WHERE a.doctor_id = " + to_string(doctorId) +
            " AND (a.appointment_date > CURDATE() OR "
            "(a.appointment_date = CURDATE() AND a.appointment_time > CURTIME())) "
            "ORDER BY a.appointment_date, a.appointment_time";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Your Schedule ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No upcoming appointments found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Appointment ID: " << row[0] << endl;
                cout << "Patient: " << row[1] << endl;
                cout << "Date: " << row[2] << endl;
                cout << "Time: " << row[3] << endl;
                cout << "Status: " << row[4] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error fetching schedule: " << mysql_error(conn) << "\n";
    }
}

void DoctorDashboard::viewPatients() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the doctor_id from the Doctors table
    string query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found in database\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now fetch the patients using the correct doctor_id
    query = "SELECT DISTINCT u.user_id, u.name, p.blood_group, p.sex "
            "FROM Users u "
            "JOIN Patients p ON u.user_id = p.user_id "
            "JOIN Appointments a ON p.patient_id = a.patient_id "
            "WHERE a.doctor_id = " + to_string(doctorId) +
            " ORDER BY u.name";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Your Patients ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No patients found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "ID: " << row[0] << endl;
                cout << "Name: " << row[1] << endl;
                cout << "Blood Group: " << row[2] << endl;
                cout << "Gender: " << row[3] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error fetching patients: " << mysql_error(conn) << "\n";
    }
}

void DoctorDashboard::prescribeMedication() {
    MYSQL* conn = Database::getConnection();
    
    // Get doctor_id first
    string doctor_query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, doctor_query.c_str())) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        cout << "Doctor not found!\n";
        mysql_free_result(result);
        return;
    }
    
    int doctor_id = stoi(row[0]);
    mysql_free_result(result);

    // Get all patients
    string query = "SELECT DISTINCT p.patient_id, u.name "
                  "FROM Patients p "
                  "JOIN Users u ON p.user_id = u.user_id "
                  "JOIN Appointments a ON p.patient_id = a.patient_id "
                  "WHERE a.doctor_id = " + to_string(doctor_id) + " "
                  "ORDER BY u.name";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "Error fetching patients: " << mysql_error(conn) << endl;
        return;
    }

    result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }

    vector<pair<int, string>> patients;
    while ((row = mysql_fetch_row(result))) {
        patients.push_back({stoi(row[0]), row[1]});
    }
    mysql_free_result(result);

    if (patients.empty()) {
        cout << "No patients found.\n";
        return;
    }

    cout << "\nSelect a patient to prescribe medication:\n";
    for (size_t i = 0; i < patients.size(); i++) {
        cout << i + 1 << ". " << patients[i].second << endl;
    }

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(patients.size())) {
        cout << "Invalid choice.\n";
        return;
    }

    int patient_id = patients[choice - 1].first;
    
    string medication_name, dosage, frequency, duration;
    cout << "Enter medication name: ";
    cin.ignore();
    getline(cin, medication_name);
    
    cout << "Enter dosage (e.g., '500mg', '1 tablet'): ";
    getline(cin, dosage);
    
    cout << "Enter frequency (e.g., 'twice daily', 'three times daily'): ";
    getline(cin, frequency);
    
    cout << "Enter duration (number of days): ";
    getline(cin, duration);

    // Calculate start and end dates
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string start_date = to_string(1900 + ltm->tm_year) + "-" + 
                       to_string(1 + ltm->tm_mon) + "-" + 
                       to_string(ltm->tm_mday);
    
    // Add duration to start date
    int duration_days = stoi(duration);
    time_t end_time = now + (duration_days * 24 * 60 * 60);
    tm* end_ltm = localtime(&end_time);
    string end_date = to_string(1900 + end_ltm->tm_year) + "-" + 
                     to_string(1 + end_ltm->tm_mon) + "-" + 
                     to_string(end_ltm->tm_mday);

    // Escape special characters
    char* escaped_medication = new char[medication_name.length() * 2 + 1];
    char* escaped_dosage = new char[dosage.length() * 2 + 1];
    char* escaped_frequency = new char[frequency.length() * 2 + 1];
    
    mysql_real_escape_string(conn, escaped_medication, medication_name.c_str(), medication_name.length());
    mysql_real_escape_string(conn, escaped_dosage, dosage.c_str(), dosage.length());
    mysql_real_escape_string(conn, escaped_frequency, frequency.c_str(), frequency.length());

    string insert_query = "INSERT INTO Medications (patient_id, doctor_id, medication_name, dosage, frequency, start_date, end_date, status) "
                         "VALUES (" + to_string(patient_id) + ", " + to_string(doctor_id) + ", '" + 
                         escaped_medication + "', '" + escaped_dosage + "', '" + escaped_frequency + 
                         "', '" + start_date + "', '" + end_date + "', 'Active')";

    delete[] escaped_medication;
    delete[] escaped_dosage;
    delete[] escaped_frequency;

    if (mysql_query(conn, insert_query.c_str())) {
        cout << "Error prescribing medication: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Medication prescribed successfully!\n";
}

void DoctorDashboard::showBalance() const {
    MYSQL* conn = Database::getConnection();
    
    // Get doctor_id
    string doctor_query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, doctor_query.c_str())) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        cout << "Doctor not found!\n";
        mysql_free_result(result);
        return;
    }
    
    int doctor_id = stoi(row[0]);
    mysql_free_result(result);

    // Get total earnings from completed appointments
    string appointment_query = "SELECT COALESCE(SUM(appointment_fees), 0) FROM Appointments a "
                             "JOIN Doctors d ON a.doctor_id = d.doctor_id "
                             "WHERE a.doctor_id = " + to_string(doctor_id) + " "
                             "AND a.status = 'completed'";
    
    if (mysql_query(conn, appointment_query.c_str())) {
        cout << "Error getting appointment earnings: " << mysql_error(conn) << endl;
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    row = mysql_fetch_row(result);
    double appointment_earnings = row[0] ? stod(row[0]) : 0;
    mysql_free_result(result);

    // Get total earnings from surgeries
    string surgery_query = "SELECT COALESCE(SUM(dsf.fee), 0) FROM DoctorSurgeryFees dsf "
                          "WHERE dsf.doctor_id = " + to_string(doctor_id);
    
    if (mysql_query(conn, surgery_query.c_str())) {
        cout << "Error getting surgery earnings: " << mysql_error(conn) << endl;
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    row = mysql_fetch_row(result);
    double surgery_earnings = row[0] ? stod(row[0]) : 0;
    mysql_free_result(result);

    // Get total withdrawals
    string withdrawal_query = "SELECT COALESCE(SUM(amount), 0) FROM DoctorTransactions "
                             "WHERE doctor_id = " + to_string(doctor_id) + " "
                             "AND type = 'withdrawal'";
    
    if (mysql_query(conn, withdrawal_query.c_str())) {
        cout << "Error getting withdrawals: " << mysql_error(conn) << endl;
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    row = mysql_fetch_row(result);
    double total_withdrawals = row[0] ? stod(row[0]) : 0;
    mysql_free_result(result);

    double current_balance = appointment_earnings + surgery_earnings - total_withdrawals;

    cout << "\n=== Current Balance ===\n";
    cout << "Appointment Earnings: " << fixed << setprecision(2) << appointment_earnings << " BDT\n";
    cout << "Surgery Earnings: " << surgery_earnings << " BDT\n";
    cout << "Total Withdrawals: " << total_withdrawals << " BDT\n";
    cout << "Current Balance: " << current_balance << " BDT\n";
    cout << "=====================\n";
}

void DoctorDashboard::showBalanceHistory() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the doctor_id from the Doctors table
    string query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found in database\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now fetch the transaction history using the correct doctor_id
    query = "SELECT amount, type, transaction_date "
            "FROM DoctorTransactions "
            "WHERE doctor_id = " + to_string(doctorId) +
            " ORDER BY transaction_date DESC";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Transaction History ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No transaction history found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Amount: $" << fixed << setprecision(2) << stod(row[0]) << endl;
                cout << "Type: " << row[1] << endl;
                cout << "Date: " << row[2] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error fetching transaction history: " << mysql_error(conn) << "\n";
    }
}

void DoctorDashboard::withdrawMoney() {
    MYSQL* conn = Database::getConnection();
    
    // Get doctor_id first
    string doctor_query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, doctor_query.c_str())) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        cout << "Doctor not found!\n";
        mysql_free_result(result);
        return;
    }
    
    int doctor_id = stoi(row[0]);
    mysql_free_result(result);

    cout << "\nEnter amount to withdraw (BDT): ";
    double amount;
    cin >> amount;
    cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    if (amount <= 0) {
        cout << "Invalid amount. Please enter a positive value.\n";
        return;
    }

    // Get current balance
    string balance_query = "SELECT "
                          "(SELECT COALESCE(SUM(d.appointment_fees), 0) FROM Appointments a "
                          "JOIN Doctors d ON a.doctor_id = d.doctor_id "
                          "WHERE a.doctor_id = " + to_string(doctor_id) + " AND a.status = 'completed') + "
                          "(SELECT COALESCE(SUM(dsf.fee), 0) FROM DoctorSurgeryFees dsf "
                          "WHERE dsf.doctor_id = " + to_string(doctor_id) + ") - "
                          "(SELECT COALESCE(SUM(amount), 0) FROM DoctorTransactions "
                          "WHERE doctor_id = " + to_string(doctor_id) + " AND type = 'withdrawal') "
                          "as current_balance";

    if (mysql_query(conn, balance_query.c_str())) {
        cout << "Error calculating balance: " << mysql_error(conn) << endl;
        return;
    }

    result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }

    row = mysql_fetch_row(result);
    double current_balance = row[0] ? stod(row[0]) : 0;
    mysql_free_result(result);

    if (amount > current_balance) {
        cout << "Insufficient balance. Your current balance is: " << fixed << setprecision(2) << current_balance << " BDT\n";
        return;
    }

    // Insert withdrawal transaction
    string insert_query = "INSERT INTO DoctorTransactions (doctor_id, amount, type, transaction_date) "
                         "VALUES (" + to_string(doctor_id) + ", " + to_string(amount) + ", 'withdrawal', NOW())";

    if (mysql_query(conn, insert_query.c_str())) {
        cout << "Error processing withdrawal: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Withdrawal of " << amount << " BDT processed successfully!\n";
    cout << "Remaining balance: " << (current_balance - amount) << " BDT\n";
}

int DoctorDashboard::calculateAge(const string& dob) const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;
    
    stringstream ss(dob);
    string year_str, month_str, day_str;
    getline(ss, year_str, '-');
    getline(ss, month_str, '-');
    getline(ss, day_str);
    
    int birthYear = stoi(year_str);
    int birthMonth = stoi(month_str);
    int birthDay = stoi(day_str);
    
    int age = currentYear - birthYear;
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }
    return age;
}

double DoctorDashboard::getTotalWithdrawn() const {
    double total = 0;
    MYSQL* conn = Database::getConnection();
    string query = "SELECT SUM(amount) FROM DoctorTransactions "
                  "WHERE doctor_id = " + to_string(doctor->getUserId()) +
                  " AND type = 'withdrawal'";

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            total = row[0] ? stod(row[0]) : 0;
        }
        mysql_free_result(result);
    }
    return total;
}

void DoctorDashboard::updateBalance(double amount, const string& type, const string& description) {
    MYSQL* conn = Database::getConnection();
    string query = "INSERT INTO DoctorTransactions (doctor_id, amount, type, description, transaction_date) "
                  "VALUES (" + to_string(doctor->getUserId()) + ", " + to_string(amount) + 
                  ", '" + type + "', '" + description + "', NOW())";

    mysql_query(conn, query.c_str());
}

bool DoctorDashboard::updateProfile() {
    MYSQL* conn = Database::getConnection();
    
    // Get doctor_id
    string doctor_query = "SELECT doctor_id FROM Doctors WHERE user_id = " + to_string(doctor->getUserId());
    if (mysql_query(conn, doctor_query.c_str())) {
        cout << "Error getting doctor ID: " << mysql_error(conn) << endl;
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        cout << "Error storing result: " << mysql_error(conn) << endl;
        return false;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        cout << "Doctor not found!\n";
        mysql_free_result(result);
        return false;
    }
    
    int doctor_id = stoi(row[0]);
    mysql_free_result(result);

    while (true) {
        cout << "\n=== Update Profile ===\n";
        cout << "1. Update Phone Number\n";
        cout << "2. Update Specialization\n";
        cout << "3. Update Appointment Fee\n";
        cout << "4. Add Degree\n";
        cout << "5. Remove Degree\n";
        cout << "6. Back to Dashboard\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                string new_phone;
                cout << "Enter new phone number (11 digits): ";
                cin >> new_phone;
                
                if (!isValidPhone(new_phone)) {
                    cout << "Invalid phone number format!\n";
                    continue;
                }

                string update_query = "UPDATE Users SET phone = '" + new_phone + "' "
                                    "WHERE user_id = " + to_string(doctor->getUserId());
                
                if (mysql_query(conn, update_query.c_str())) {
                    cout << "Error updating phone: " << mysql_error(conn) << endl;
                    continue;
                }
                cout << "Phone number updated successfully!\n";
                break;
            }
            case 2: {
                string new_specialization;
                cout << "Enter new specialization: ";
                cin.ignore();
                getline(cin, new_specialization);
                
                if (new_specialization.length() < 3) {
                    cout << "Specialization must be at least 3 characters long!\n";
                    continue;
                }

                string update_query = "UPDATE Doctors SET specialization = '" + new_specialization + "' "
                                    "WHERE doctor_id = " + to_string(doctor_id);
                
                if (mysql_query(conn, update_query.c_str())) {
                    cout << "Error updating specialization: " << mysql_error(conn) << endl;
                    continue;
                }
                cout << "Specialization updated successfully!\n";
                break;
            }
            case 3: {
                double new_fee;
                cout << "Enter new appointment fee (100-5000 BDT): ";
                cin >> new_fee;
                
                if (new_fee < 100 || new_fee > 5000) {
                    cout << "Appointment fee must be between 100 and 5000 BDT!\n";
                    continue;
                }

                string update_query = "UPDATE Doctors SET appointment_fees = " + to_string(new_fee) + " "
                                    "WHERE doctor_id = " + to_string(doctor_id);
                
                if (mysql_query(conn, update_query.c_str())) {
                    cout << "Error updating appointment fee: " << mysql_error(conn) << endl;
                    continue;
                }
                cout << "Appointment fee updated successfully!\n";
                break;
            }
            case 4: {
                string new_degree;
                cout << "Enter new degree: ";
                cin.ignore();
                getline(cin, new_degree);
                
                if (new_degree.empty()) {
                    cout << "Degree cannot be empty!\n";
                    continue;
                }

                string insert_query = "INSERT INTO DoctorDegrees (doctor_id, degree) VALUES (" + 
                                    to_string(doctor_id) + ", '" + new_degree + "')";
                
                if (mysql_query(conn, insert_query.c_str())) {
                    cout << "Error adding degree: " << mysql_error(conn) << endl;
                    continue;
                }
                cout << "Degree added successfully!\n";
                break;
            }
            case 5: {
                // First show current degrees
                string degrees_query = "SELECT degree_id, degree FROM DoctorDegrees WHERE doctor_id = " + 
                                     to_string(doctor_id);
                
                if (mysql_query(conn, degrees_query.c_str())) {
                    cout << "Error fetching degrees: " << mysql_error(conn) << endl;
                    continue;
                }
                
                result = mysql_store_result(conn);
                if (!result) {
                    cout << "Error storing result: " << mysql_error(conn) << endl;
                    continue;
                }
                
                if (mysql_num_rows(result) == 0) {
                    cout << "No degrees found!\n";
                    mysql_free_result(result);
                    continue;
                }
                
                cout << "\nCurrent Degrees:\n";
                while ((row = mysql_fetch_row(result))) {
                    cout << row[0] << ". " << row[1] << endl;
                }
                mysql_free_result(result);
                
                int degree_id;
                cout << "Enter degree ID to remove: ";
                cin >> degree_id;
                
                string delete_query = "DELETE FROM DoctorDegrees WHERE doctor_id = " + to_string(doctor_id) + 
                                    " AND degree_id = " + to_string(degree_id);
                
                if (mysql_query(conn, delete_query.c_str())) {
                    cout << "Error removing degree: " << mysql_error(conn) << endl;
                    continue;
                }
                cout << "Degree removed successfully!\n";
                break;
            }
            default:
                cout << "Invalid choice!\n";
        }
    }
    return true;
}

bool DoctorDashboard::isValidPhone(const string& phone) {
    return phone.length() == 11 && std::all_of(phone.begin(), phone.end(), ::isdigit);
}

string DoctorDashboard::getDepartmentName(int department_id) const {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT name FROM Departments WHERE department_id = " + to_string(department_id);
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row) {
                string name = row[0];
                mysql_free_result(result);
                return name;
            }
            mysql_free_result(result);
        }
    }
    return "Unknown Department";
} 