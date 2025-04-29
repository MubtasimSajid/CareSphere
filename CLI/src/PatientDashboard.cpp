#define _CRT_SECURE_NO_WARNINGS
#include "../include/PatientDashboard.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mysql.h>
#include <ctime>

PatientDashboard::PatientDashboard(int userId) : patient(new Patient()) {
    loadPatientData(userId);
}

PatientDashboard::~PatientDashboard() {
    if (patient) delete patient;
}

void PatientDashboard::loadPatientData(int userId) {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT u.user_id, u.name, u.email, u.phone, u.dob, "
                  "p.sex, p.blood_group, p.weight, p.height, p.blood_pressure, p.blood_sugar "
                  "FROM Users u "
                  "JOIN Patients p ON u.user_id = p.user_id "
                  "WHERE u.user_id = " + to_string(userId);

    cout << "Loading patient data for user ID: " << userId << endl;
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            cout << "Found patient data in database" << endl;
            
            // Set base class (User) data
            patient->setUserId(stoi(row[0]));
            patient->setName(row[1]);
            patient->setEmail(row[2]);
            patient->setPhone(row[3]);
            patient->setDOB(row[4]);
            
            // Set Patient-specific data
            patient->setGender(row[5]);
            patient->setBloodGroup(row[6]);
            
            // Add NULL checks and debug output for numeric fields
            cout << "Weight: " << (row[7] ? row[7] : "NULL") << endl;
            cout << "Height: " << (row[8] ? row[8] : "NULL") << endl;
            cout << "Blood Pressure: " << (row[9] ? row[9] : "NULL") << endl;
            cout << "Blood Sugar: " << (row[10] ? row[10] : "NULL") << endl;
            
            // Set numeric values with default values if NULL
            patient->setWeight(row[7] ? stod(row[7]) : 0.0);
            patient->setHeight(row[8] ? stod(row[8]) : 0.0);
            patient->setBloodPressure(row[9] ? row[9] : "N/A");
            patient->setBloodSugar(row[10] ? stod(row[10]) : 0.0);
            
            cout << "Patient data loaded successfully" << endl;
        } else {
            cout << "No patient data found for user ID: " << userId << endl;
        }
        mysql_free_result(result);
    } else {
        cout << "Error querying patient data: " << mysql_error(conn) << endl;
    }

    // Load allergies
    query = "SELECT allergy_name FROM PatientAllergies WHERE patient_id = (SELECT patient_id FROM Patients WHERE user_id = " + to_string(userId) + ")";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            patient->addAllergy(row[0]);
        }
        mysql_free_result(result);
    }

    // Load chronic conditions
    query = "SELECT condition_name FROM PatientChronicConditions WHERE patient_id = (SELECT patient_id FROM Patients WHERE user_id = " + to_string(userId) + ")";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            patient->addChronicCondition(row[0]);
        }
        mysql_free_result(result);
    }
    
    cout << "Patient data loading completed" << endl;
}

void PatientDashboard::displayDashboard() {
    while (true) {
        cout << "\n=================================\n";
        cout << "     Patient Dashboard Menu\n";
        cout << "=================================\n";
        cout << "1. View Profile\n";
        cout << "2. View Medical Information\n";
        cout << "3. View Advanced Analysis\n";
        cout << "4. Make a Note\n";
        cout << "5. View Notes\n";
        cout << "6. View Schedule\n";
        cout << "7. Make Appointment\n";
        cout << "8. View Medications\n";
        cout << "9. View Unpaid Bills\n";
        cout << "10. Pay Bill\n";
        cout << "11. Rate Doctor\n";
        cout << "12. Rate Hospital\n";
        cout << "13. Logout\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: showProfile(); Sleep(2000);  break;
            case 2: showMedicalInfo(); Sleep(2000); break;
            case 3: showAdvancedAnalysis(); Sleep(2000); break;
            case 4: makeNote(); Sleep(2000); break;
            case 5: showNotes(); Sleep(2000); break;
            case 6: showSchedule(); Sleep(2000); break;
            case 7: makeAppointment(); Sleep(2000); break;
            case 8: showMedications(); Sleep(2000); break;
            case 9: showUnpaidBills(); Sleep(2000); break;
            case 10: {
                cout << "Enter bill ID: ";
                int billId;
                cin >> billId;
                cin.ignore();
                payBill(billId);
                Sleep(2000);
                break;
            }
            case 11: {
                cout << "Enter Doctor ID: ";
                int doctorId;
                cin >> doctorId;
                cin.ignore();
                rateDoctor(doctorId);
                Sleep(2000);
                break;
            }
            case 12: {
                cout << "Enter Hospital ID: ";
                int hospitalId;
                cin >> hospitalId;
                cin.ignore();
                rateHospital(hospitalId);
                Sleep(2000);
                break;
            }
            case 13: return;
            default: cout << "Invalid choice!\n";
        }
    }
}

void PatientDashboard::showProfile() const {
    cout << "\n=== Patient Profile ===\n";
    cout << "Name: " << patient->getName() << endl;
    cout << "Email: " << patient->getEmail() << endl;
    cout << "Phone: " << patient->getPhone() << endl;
    cout << "Age: " << calculateAge() << endl;
}

void PatientDashboard::showMedicalInfo() const {
    cout << "\n=== Medical Information ===\n";
    cout << "Gender: " << patient->getGender() << endl;
    cout << "Weight: " << patient->getWeight() << " kg" << endl;
    cout << "Height: " << patient->getHeight() << " cm" << endl;
    cout << "Blood Sugar: " << patient->getBloodSugar() << endl;
    cout << "Blood Pressure: " << patient->getBloodPressure() << endl;

    cout << "\nAllergies:\n";
    for (const auto& allergy : patient->getAllergies()) {
        cout << "- " << allergy << endl;
    }

    cout << "\nChronic Conditions:\n";
    for (const auto& condition : patient->getChronicConditions()) {
        cout << "- " << condition << endl;
    }
}

void PatientDashboard::showAdvancedAnalysis() const {
    double bmi = calculateBMI();
    string category = getBMICategory(bmi);
    cout << "\n=== Advanced Health Analysis ===\n";
    cout << "BMI: " << fixed << setprecision(2) << bmi << endl;
    cout << "Category: " << category << endl;

    cout << "\nBlood Pressure Analysis:\n";
    if (isHighBloodPressure()) {
        cout << "Warning: High blood pressure detected!\n";
    } else if (isLowBloodPressure()) {
        cout << "Warning: Low blood pressure detected!\n";
    } else {
        cout << "Blood pressure is normal.\n";
    }

    cout << "\nBlood Sugar Analysis:\n";
    if (isDiabetic()) {
        cout << "Warning: Blood sugar levels indicate diabetic range!\n";
    } else {
        cout << "Blood sugar levels are normal.\n";
    }
}

void PatientDashboard::makeNote() {
    cout << "\nEnter your note: ";
    string note;
    getline(cin, note);

    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Escape special characters in the note
    char* escapedNote = new char[note.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedNote, note.c_str(), note.length());
    
    // Insert the note using patient_id
    query = "INSERT INTO PatientNotes (patient_id, note, created_at) VALUES (" +
            to_string(patientId) + ", '" + escapedNote + "', NOW())";
    
    delete[] escapedNote;
    
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Note added successfully!\n";
    } else {
        cout << "Error adding note: " << mysql_error(conn) << "\n";
    }
}

void PatientDashboard::showNotes() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now get the notes using patient_id
    query = "SELECT note, created_at FROM PatientNotes "
            "WHERE patient_id = " + to_string(patientId) +
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
        cout << "Error retrieving notes: " << mysql_error(conn) << "\n";
    }
}

void PatientDashboard::showSchedule() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now get the appointments using patient_id
    query = "SELECT a.appointment_id, u.name, a.appointment_date, a.appointment_time, a.status "
            "FROM Appointments a "
            "JOIN Doctors d ON a.doctor_id = d.doctor_id "
            "JOIN Users u ON d.user_id = u.user_id "
            "WHERE a.patient_id = " + to_string(patientId) +
            " ORDER BY a.appointment_date, a.appointment_time";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Your Schedule ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No appointments found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Appointment ID: " << row[0] << endl;
                cout << "Doctor: " << row[1] << endl;
                cout << "Date: " << row[2] << endl;
                cout << "Time: " << row[3] << endl;
                cout << "Status: " << row[4] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error retrieving schedule: " << mysql_error(conn) << "\n";
    }
}

void PatientDashboard::makeAppointment() {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Step 1: Display all departments
    cout << "\n=== Available Departments ===\n";
    query = "SELECT department_id, name FROM Departments ORDER BY name";
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching departments: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    while ((row = mysql_fetch_row(result))) {
        cout << row[0] << ". " << row[1] << endl;
    }
    mysql_free_result(result);
    
    // Get department choice and validate
    int departmentId;
    do {
        cout << "\nEnter Department ID: ";
        cin >> departmentId;
        cin.ignore();
    } while (!validateDepartment(departmentId));
    
    // Step 2: Show top 5 doctors in the selected department based on ratings
    cout << "\n=== Top Rated Doctors in " << getDepartmentName(conn, departmentId) << " ===\n";
    query = "SELECT d.doctor_id, u.name, d.specialization, d.appointment_fees, "
            "d.average_rating, d.total_ratings, "
            "GROUP_CONCAT(DISTINCT h.hospital_name) as hospitals "
            "FROM Doctors d "
            "JOIN Users u ON d.user_id = u.user_id "
            "LEFT JOIN DoctorHospitalAffiliations dha ON d.doctor_id = dha.doctor_id "
            "LEFT JOIN Hospitals h ON dha.hospital_id = h.hospital_id "
            "WHERE d.department_id = " + to_string(departmentId) + " "
            "GROUP BY d.doctor_id "
            "ORDER BY d.average_rating DESC, d.total_ratings DESC "
            "LIMIT 5";
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching doctors: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No doctors found in this department.\n";
        mysql_free_result(result);
        return;
    }
    
    while ((row = mysql_fetch_row(result))) {
        cout << "\nDoctor ID: " << row[0] << endl;
        cout << "Name: " << row[1] << endl;
        cout << "Specialization: " << row[2] << endl;
        cout << "Fee: $" << fixed << setprecision(2) << stod(row[3]) << endl;
        cout << "Rating: " << (row[4] ? row[4] : "No ratings yet") << " (" 
             << (row[5] ? row[5] : "0") << " ratings)" << endl;
        cout << "Available at: " << (row[6] ? row[6] : "Private Clinic") << endl;
        cout << "-------------------\n";
    }
    mysql_free_result(result);
    
    // Get doctor choice and validate
    int doctorId;
    do {
        cout << "\nEnter Doctor ID: ";
        cin >> doctorId;
        cin.ignore();
    } while (!validateDoctor(doctorId, departmentId));
    
    // Step 3: Show available hospitals for the selected doctor
    cout << "\n=== Available Locations ===\n";
    query = "SELECT h.hospital_id, h.hospital_name, h.address, h.average_rating, h.total_ratings "
            "FROM Hospitals h "
            "JOIN DoctorHospitalAffiliations dha ON h.hospital_id = dha.hospital_id "
            "WHERE dha.doctor_id = " + to_string(doctorId);
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching hospitals: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "Doctor is available at Private Clinic\n";
        cout << "Press Enter to continue...";
        cin.get();
    } else {
        while ((row = mysql_fetch_row(result))) {
            cout << "\nHospital ID: " << row[0] << endl;
            cout << "Name: " << row[1] << endl;
            cout << "Address: " << row[2] << endl;
            cout << "Rating: " << (row[3] ? row[3] : "No ratings yet") << " (" 
                 << (row[4] ? row[4] : "0") << " ratings)" << endl;
            cout << "-------------------\n";
        }
        mysql_free_result(result);
    }
    
    // Get location choice and validate
    int locationId;
    do {
        cout << "\nEnter Hospital ID (or 0 for Private Clinic): ";
        cin >> locationId;
        cin.ignore();
    } while (!validateHospital(locationId, doctorId));
    
    // Step 4: Get appointment date and time
    string appointmentDate, appointmentTime;
    do {
        cout << "\nEnter Appointment Date (YYYY-MM-DD): ";
        getline(cin, appointmentDate);
        
        cout << "Enter Appointment Time (HH:MM): ";
        getline(cin, appointmentTime);
    } while (!validateAppointmentDateTime(appointmentDate, appointmentTime));
    
    // Check if the time slot is available
    query = "SELECT COUNT(*) FROM Appointments "
            "WHERE doctor_id = " + to_string(doctorId) + " "
            "AND appointment_date = '" + appointmentDate + "' "
            "AND appointment_time = '" + appointmentTime + "' "
            "AND status != 'Cancelled'";
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error checking appointment availability: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    bool isAvailable = stoi(row[0]) == 0;
    mysql_free_result(result);
    
    if (!isAvailable) {
        cout << "This time slot is already booked. Please choose another time.\n";
        return;
    }
    
    // Step 5: Create the appointment
    query = "INSERT INTO Appointments (doctor_id, patient_id, appointment_date, appointment_time) "
            "VALUES (" + to_string(doctorId) + ", " + to_string(patientId) + ", '" + 
            appointmentDate + "', '" + appointmentTime + "')";
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error creating appointment: " << mysql_error(conn) << "\n";
        return;
    }
    
    int appointmentId = mysql_insert_id(conn);
    
    // Step 6: Create bill
    query = "SELECT appointment_fees FROM Doctors WHERE doctor_id = " + to_string(doctorId);
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor fees: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    double fee = stod(row[0]);
    mysql_free_result(result);
    
    // Get doctor's name for bill description
    query = "SELECT u.name FROM Users u JOIN Doctors d ON u.user_id = d.user_id WHERE d.doctor_id = " + to_string(doctorId);
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor name: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    string doctorName = row[0];
    mysql_free_result(result);
    
    // Create bill
    string billDescription = "Appointment with Dr. " + doctorName;
    query = "INSERT INTO Bills (patient_id, amount, description, issue_date, due_date) "
            "VALUES (" + to_string(patientId) + ", " + to_string(fee) + ", '" + 
            billDescription + "', CURDATE(), DATE_ADD(CURDATE(), INTERVAL 30 DAY))";
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error creating bill: " << mysql_error(conn) << "\n";
        return;
    }
    
    cout << "\nAppointment scheduled successfully!\n";
    cout << "Bill created: $" << fixed << setprecision(2) << fee << endl;
}

string PatientDashboard::getDepartmentName(MYSQL* conn, int departmentId) {
    string query = "SELECT name FROM Departments WHERE department_id = " + to_string(departmentId);
    if (mysql_query(conn, query.c_str()) != 0) {
        return "Unknown Department";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    string name = row ? row[0] : "Unknown Department";
    mysql_free_result(result);
    return name;
}

void PatientDashboard::showUnpaidBills() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now get the unpaid bills using patient_id
    query = "SELECT b.bill_id, b.amount, b.description, b.due_date "
            "FROM Bills b "
            "WHERE b.patient_id = " + to_string(patientId) +
            " AND b.status = 'Pending' "
            "ORDER BY b.due_date";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Unpaid Bills ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No unpaid bills found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Bill ID: " << row[0] << endl;
                cout << "Amount: $" << row[1] << endl;
                cout << "Description: " << row[2] << endl;
                cout << "Due Date: " << row[3] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error retrieving bills: " << mysql_error(conn) << "\n";
    }
}

void PatientDashboard::payBill(int billId) {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Verify bill exists and belongs to patient
    query = "SELECT amount FROM Bills "
            "WHERE bill_id = " + to_string(billId) +
            " AND patient_id = " + to_string(patientId) +
            " AND status = 'Pending'";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            double amount = stod(row[0]);
            
            cout << "Bill amount: $" << fixed << setprecision(2) << amount << endl;
            cout << "Confirm payment? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();

            if (tolower(confirm) == 'y') {
                query = "UPDATE Bills SET status = 'Paid' "
                        "WHERE bill_id = " + to_string(billId);

                if (mysql_query(conn, query.c_str()) == 0) {
                    cout << "Payment successful!\n";
                } else {
                    cout << "Error processing payment: " << mysql_error(conn) << "\n";
                }
            }
        } else {
            cout << "Invalid bill ID or bill already paid.\n";
        }
        mysql_free_result(result);
    } else {
        cout << "Error verifying bill: " << mysql_error(conn) << "\n";
    }
}

void PatientDashboard::showMedications() const {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Now get the medications using patient_id
    query = "SELECT m.medication_name, m.dosage, m.frequency, m.start_date, m.end_date, m.status, u.name "
            "FROM Medications m "
            "JOIN Doctors d ON m.doctor_id = d.doctor_id "
            "JOIN Users u ON d.user_id = u.user_id "
            "WHERE m.patient_id = " + to_string(patientId) +
            " ORDER BY m.start_date DESC";

    if (mysql_query(conn, query.c_str()) == 0) {
        result = mysql_store_result(conn);
        cout << "\n=== Your Medications ===\n";
        if (mysql_num_rows(result) == 0) {
            cout << "No medications found.\n";
        } else {
            while ((row = mysql_fetch_row(result))) {
                cout << "Medication: " << row[0] << endl;
                cout << "Dosage: " << row[1] << endl;
                cout << "Frequency: " << row[2] << endl;
                cout << "Start Date: " << row[3] << endl;
                cout << "End Date: " << (row[4] ? row[4] : "Ongoing") << endl;
                cout << "Status: " << row[5] << endl;
                cout << "Prescribed By: Dr. " << row[6] << endl;
                cout << "-------------------\n";
            }
        }
        mysql_free_result(result);
    } else {
        cout << "Error retrieving medications: " << mysql_error(conn) << "\n";
    }
}

double PatientDashboard::calculateBMI() const {
    double heightInMeters = patient->getHeight() / 100.0;  // Convert cm to meters
    return patient->getWeight() / (heightInMeters * heightInMeters);
}

string PatientDashboard::getBMICategory(double bmi) const {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25.0) return "Normal weight";
    if (bmi < 30.0) return "Overweight";
    return "Obese";
}

bool PatientDashboard::isHighBloodPressure() const {
    string bp = patient->getBloodPressure();
    size_t pos = bp.find('/');
    int systolic = stoi(bp.substr(0, pos));
    int diastolic = stoi(bp.substr(pos + 1));
    return systolic >= 140 || diastolic >= 90;
}

bool PatientDashboard::isLowBloodPressure() const {
    string bp = patient->getBloodPressure();
    size_t pos = bp.find('/');
    int systolic = stoi(bp.substr(0, pos));
    int diastolic = stoi(bp.substr(pos + 1));
    return systolic < 90 || diastolic < 60;
}

bool PatientDashboard::isDiabetic() const {
    return patient->getBloodSugar() > 126;  // Fasting blood sugar > 126 mg/dL indicates diabetes
}

string PatientDashboard::getCurrentDateTime() const {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
       << setfill('0') << setw(2) << ltm->tm_mday << " "
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min;
    return ss.str();
}

bool PatientDashboard::validateAppointmentDateTime(const string& date, const string& time) const {
    // Basic format validation
    if (date.length() != 10 || time.length() != 5) {
        cout << "Invalid date or time format. Use YYYY-MM-DD for date and HH:MM for time.\n";
        return false;
    }

    // Check date format (YYYY-MM-DD)
    if (date[4] != '-' || date[7] != '-') {
        cout << "Invalid date format. Use YYYY-MM-DD.\n";
        return false;
    }

    // Check time format (HH:MM)
    if (time[2] != ':') {
        cout << "Invalid time format. Use HH:MM.\n";
        return false;
    }

    try {
        // Parse date components
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));
        int hour = std::stoi(time.substr(0, 2));
        int minute = std::stoi(time.substr(3, 2));

        // Validate ranges
        if (month < 1 || month > 12 || day < 1 || day > 31 || 
            hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            cout << "Invalid date or time values.\n";
            return false;
        }

        // Create tm structure
        tm appointment = {};
        appointment.tm_year = year - 1900;
        appointment.tm_mon = month - 1;
        appointment.tm_mday = day;
        appointment.tm_hour = hour;
        appointment.tm_min = minute;
        appointment.tm_sec = 0;

        // Convert to time_t
        time_t appointmentTime = mktime(&appointment);
        time_t now = std::time(nullptr);

        // Check if appointment is in the future
        if (appointmentTime <= now) {
            cout << "Appointment time must be in the future.\n";
            return false;
        }

        // Note: The time slot availability check should be done in makeAppointment
        // since we don't have access to doctorId here
        return true;
    }
    catch (const std::exception&) {
        cout << "Error parsing date or time.\n";
        return false;
    }
}

double PatientDashboard::calculateAppointmentFee(int doctorId) const {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT appointment_fees FROM Doctors WHERE user_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(result)) {
            double fee = stod(row[0]);
            mysql_free_result(result);
            return fee;
        }
        mysql_free_result(result);
    }
    return 0.0;
}

int PatientDashboard::calculateAge() const {
    string dob = patient->getDOB();
    if (dob.empty()) {
        cout << "Warning: Date of birth not set" << endl;
        return 0;
    }
    return patient->calculateAge(dob);
}

void PatientDashboard::rateDoctor(int doctorId) {
    MYSQL* conn = Database::getConnection();
    
    // First get the patient_id from the Patients table
    string query = "SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId());
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting patient ID: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Patient not found\n";
        mysql_free_result(result);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientId = stoi(row[0]);
    mysql_free_result(result);
    
    // Get doctor's name
    query = "SELECT u.name FROM Users u JOIN Doctors d ON u.user_id = d.user_id WHERE d.doctor_id = " + to_string(doctorId);
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting doctor name: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result || !mysql_num_rows(result)) {
        cout << "Error: Doctor not found\n";
        mysql_free_result(result);
        return;
    }
    
    row = mysql_fetch_row(result);
    string doctorName = row[0];
    mysql_free_result(result);
    
    cout << "Rate Dr. " << doctorName << endl;
    
    double rating;
    cout << "Enter rating (1-5): ";
    cin >> rating;
    cin.ignore();
    
    if (rating < 1 || rating > 5) {
        cout << "Invalid rating! Rating must be between 1 and 5.\n";
        return;
    }
    
    string review;
    cout << "Enter your review (optional): ";
    getline(cin, review);
    
    // Escape special characters in the review
    char* escaped_review = new char[review.length() * 2 + 1];
    mysql_real_escape_string(conn, escaped_review, review.c_str(), review.length());
    
    // Insert the review
    query = "INSERT INTO DoctorReviews (doctor_id, patient_id, rating, review_text, review_date) "
            "VALUES (" + to_string(doctorId) + ", " + to_string(patientId) + ", " + 
            to_string(rating) + ", '" + escaped_review + "', NOW())";
    
    delete[] escaped_review;
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error submitting rating: " << mysql_error(conn) << "\n";
        return;
    }
    
    // Update doctor's average rating
    query = "UPDATE Doctors d SET "
            "average_rating = (SELECT AVG(rating) FROM DoctorReviews WHERE doctor_id = " + to_string(doctorId) + "), "
            "total_ratings = (SELECT COUNT(*) FROM DoctorReviews WHERE doctor_id = " + to_string(doctorId) + ") "
            "WHERE d.doctor_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error updating doctor rating: " << mysql_error(conn) << "\n";
        return;
    }
    
    cout << "Rating submitted successfully!\n";
}

void PatientDashboard::rateHospital(int hospitalId) {
    MYSQL* conn = Database::getConnection();
    
    // First verify if patient has had an appointment at this hospital
    string query = "SELECT COUNT(*) FROM Appointments a "
                  "JOIN DoctorHospitalAffiliations dha ON a.doctor_id = dha.doctor_id "
                  "WHERE dha.hospital_id = " + to_string(hospitalId) + " "
                  "AND a.patient_id = (SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId()) + ") "
                  "AND a.status = 'Completed'";
                  
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error checking appointment history: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    int appointmentCount = stoi(row[0]);
    mysql_free_result(result);
    
    if (appointmentCount == 0) {
        cout << "You can only rate hospitals where you have had appointments.\n";
        return;
    }
    
    // Get hospital name for display
    query = "SELECT hospital_name FROM Hospitals WHERE hospital_id = " + to_string(hospitalId);
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error getting hospital name: " << mysql_error(conn) << "\n";
        return;
    }
    
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    string hospitalName = row[0];
    mysql_free_result(result);
    
    // Get rating from user
    cout << "\nRate " << hospitalName << "\n";
    cout << "Enter rating (1-5): ";
    int rating;
    cin >> rating;
    cin.ignore();
    
    if (rating < 1 || rating > 5) {
        cout << "Invalid rating. Please enter a number between 1 and 5.\n";
        return;
    }
    
    // Get review comment
    cout << "Enter your review (optional): ";
    string review;
    getline(cin, review);
    
    // Escape special characters in review
    char* escapedReview = new char[review.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedReview, review.c_str(), review.length());
    
    // Insert rating and review
    query = "INSERT INTO HospitalReviews (hospital_id, patient_id, rating, review, review_date) "
            "VALUES (" + to_string(hospitalId) + ", "
            "(SELECT patient_id FROM Patients WHERE user_id = " + to_string(patient->getUserId()) + "), "
            + to_string(rating) + ", '" + escapedReview + "', NOW())";
            
    delete[] escapedReview;
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error submitting rating: " << mysql_error(conn) << "\n";
        return;
    }
    
    // Update hospital's average rating
    query = "UPDATE Hospitals SET "
            "average_rating = (SELECT AVG(rating) FROM HospitalReviews WHERE hospital_id = " + to_string(hospitalId) + "), "
            "total_ratings = (SELECT COUNT(*) FROM HospitalReviews WHERE hospital_id = " + to_string(hospitalId) + ") "
            "WHERE hospital_id = " + to_string(hospitalId);
            
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error updating hospital rating: " << mysql_error(conn) << "\n";
        return;
    }
    
    cout << "Thank you for your rating!\n";
}

bool PatientDashboard::validateDepartment(int departmentId) const {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT COUNT(*) FROM Departments WHERE department_id = " + to_string(departmentId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error validating department: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = stoi(row[0]) > 0;
    mysql_free_result(result);
    
    if (!exists) {
        cout << "Invalid department ID. Please select a valid department.\n";
    }
    
    return exists;
}

bool PatientDashboard::validateDoctor(int doctorId, int departmentId) const {
    MYSQL* conn = Database::getConnection();
    string query = "SELECT COUNT(*) FROM Doctors WHERE doctor_id = " + to_string(doctorId) + 
                  " AND department_id = " + to_string(departmentId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error validating doctor: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = stoi(row[0]) > 0;
    mysql_free_result(result);
    
    if (!exists) {
        cout << "Invalid doctor ID or doctor does not belong to selected department.\n";
    }
    
    return exists;
}

bool PatientDashboard::validateHospital(int hospitalId, int doctorId) const {
    if (hospitalId == 0) return true; // Private clinic is always valid
    
    MYSQL* conn = Database::getConnection();
    string query = "SELECT COUNT(*) FROM DoctorHospitalAffiliations "
                  "WHERE hospital_id = " + to_string(hospitalId) + 
                  " AND doctor_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error validating hospital: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = stoi(row[0]) > 0;
    mysql_free_result(result);
    
    if (!exists) {
        cout << "Invalid hospital ID or doctor does not practice at this hospital.\n";
    }
    
    return exists;
}