#define _CRT_SECURE_NO_WARNINGS
#include "../include/HospitalDashboard.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mysql.h>
#include <ctime>

HospitalDashboard::HospitalDashboard(int userId) : hospital(new Hospital()) {
    conn = Database::getConnection();
    if (!hospital->loadHospitalData(userId)) {
        cout << "Error loading hospital data.\n";
        exit(1);
    }
}

HospitalDashboard::~HospitalDashboard() {
    if (hospital) delete hospital;
}

void HospitalDashboard::displayDashboard() {
    while (true) {
        cout << "\n=================================\n";
        cout << "     Hospital Dashboard Menu\n";
        cout << "=================================\n";
        cout << "1. View Profile\n";
        cout << "2. View Departments\n";
        cout << "3. View Doctors\n";
        cout << "4. View Ambulances\n";
        cout << "5. View Medical Services\n";
        cout << "6. View Financials\n";
        cout << "7. View Ratings\n";
        cout << "8. Add Department\n";
        cout << "9. Remove Department\n";
        cout << "10. Add Doctor\n";
        cout << "11. Remove Doctor\n";
        cout << "12. Add Ambulance\n";
        cout << "13. Remove Ambulance\n";
        cout << "14. Add Medical Service\n";
        cout << "15. Remove Medical Service\n";
        cout << "16. Update Department Fees\n";
        cout << "17. Update Surgery Fees\n";
        cout << "18. Update Appointment Fees\n";
        cout << "19. View Appointments\n";
        cout << "20. View Bills\n";
        cout << "21. View Payments\n";
        cout << "22. Generate Report\n";
        cout << "23. Logout\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: showProfile(); break;
            case 2: showDepartments(); break;
            case 3: showDoctors(); break;
            case 4: showAmbulances(); break;
            case 5: showMedicalServices(); break;
            case 6: showFinancials(); break;
            case 7: showRatings(); break;
            case 8: addDepartment(); break;
            case 9: removeDepartment(); break;
            case 10: addDoctor(); break;
            case 11: removeDoctor(); break;
            case 12: addAmbulance(); break;
            case 13: removeAmbulance(); break;
            case 14: addMedicalService(); break;
            case 15: removeMedicalService(); break;
            case 16: updateDepartmentFees(); break;
            case 17: updateSurgeryFees(); break;
            case 18: updateAppointmentFees(); break;
            case 19: viewAppointments(); break;
            case 20: viewBills(); break;
            case 21: viewPayments(); break;
            case 22: generateReport(); break;
            case 23: return;
            default: cout << "Invalid choice!\n";
        }
    }
}

void HospitalDashboard::showProfile() const {
    hospital->displayInfo();
}

void HospitalDashboard::showDepartments() const {
    hospital->displayDepartments();
}

void HospitalDashboard::showDoctors() const {
    hospital->displayDoctors();
}

void HospitalDashboard::showAmbulances() const {
    hospital->displayAmbulances();
}

void HospitalDashboard::showMedicalServices() const {
    hospital->displayMedicalServices();
}

void HospitalDashboard::showFinancials() const {
    cout << "\n=== Financial Information ===\n";
    cout << "Income: $" << fixed << setprecision(2) << hospital->getIncome() << endl;
    cout << "Expenditure: $" << fixed << setprecision(2) << hospital->getExpenditure() << endl;
    cout << "Net Balance: $" << fixed << setprecision(2) << (hospital->getIncome() - hospital->getExpenditure()) << endl;
}

void HospitalDashboard::showRatings() const {
    cout << "\n=== Hospital Ratings ===\n";
    cout << "Average Rating: " << fixed << setprecision(2) << hospital->getAverageRating() << endl;
    cout << "Total Ratings: " << hospital->getTotalRatings() << endl;
}

void HospitalDashboard::addDepartment() {
    cout << "\n=== Add Department ===\n";
    cout << "Available Departments:\n";
    
    string query = "SELECT department_id, name FROM Departments";
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching departments: " << mysql_error(conn) << "\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << row[0] << ". " << row[1] << endl;
    }
    mysql_free_result(result);

    int departmentId;
    cout << "\nEnter Department ID: ";
    cin >> departmentId;
    cin.ignore();

    if (!validateDepartment(departmentId)) {
        cout << "Invalid department ID.\n";
        return;
    }

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
    char* escapedName = new char[departmentName.length() * 2 + 1];
    char* escapedDesc = new char[description.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedName, departmentName.c_str(), departmentName.length());
    mysql_real_escape_string(conn, escapedDesc, description.c_str(), description.length());

    query = "INSERT INTO HospitalDepartments (hospital_id, department_id, department_name, department_description, appointment_fee) "
            "VALUES (" + to_string(hospital->getUserId()) + ", " + to_string(departmentId) + ", '" + 
            escapedName + "', '" + escapedDesc + "', " + to_string(appointmentFee) + ")";

    delete[] escapedName;
    delete[] escapedDesc;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Department added successfully!\n";
    } else {
        cout << "Error adding department: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::removeDepartment() {
    cout << "\n=== Remove Department ===\n";
    showDepartments();

    int departmentId;
    cout << "\nEnter Department ID to remove: ";
    cin >> departmentId;
    cin.ignore();

    if (!validateDepartment(departmentId)) {
        cout << "Invalid department ID.\n";
        return;
    }

    string query = "DELETE FROM HospitalDepartments WHERE hospital_id = " + to_string(hospital->getUserId()) +
                  " AND department_id = " + to_string(departmentId);

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Department removed successfully!\n";
    } else {
        cout << "Error removing department: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::addDoctor() {
    cout << "\n=== Add Doctor ===\n";
    
    int doctorId;
    cout << "Enter Doctor ID: ";
    cin >> doctorId;
    cin.ignore();

    if (!validateDoctor(doctorId)) {
        cout << "Invalid doctor ID.\n";
        return;
    }

    string query = "INSERT INTO DoctorHospitalAffiliations (doctor_id, hospital_id) "
            "VALUES (" + to_string(doctorId) + ", " + to_string(hospital->getUserId()) + ")";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Doctor added successfully!\n";
    } else {
        cout << "Error adding doctor: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::removeDoctor() {
    cout << "\n=== Remove Doctor ===\n";
    showDoctors();

    int doctorId;
    cout << "\nEnter Doctor ID to remove: ";
    cin >> doctorId;
    cin.ignore();

    if (!validateDoctor(doctorId)) {
        cout << "Invalid doctor ID.\n";
        return;
    }

    string query = "DELETE FROM DoctorHospitalAffiliations WHERE hospital_id = " + to_string(hospital->getUserId()) +
                  " AND doctor_id = " + to_string(doctorId);

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Doctor removed successfully!\n";
    } else {
        cout << "Error removing doctor: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::addAmbulance() {
    cout << "\n=== Add Ambulance ===\n";
    
    string vehicleNumber;
    cout << "Enter Vehicle Number: ";
    getline(cin, vehicleNumber);

    string phoneNumber;
    cout << "Enter Phone Number: ";
    getline(cin, phoneNumber);

    if (!validateAmbulanceDetails(vehicleNumber, phoneNumber)) {
        cout << "Invalid ambulance details.\n";
        return;
    }

    double feePerKm;
    cout << "Enter Fee per Kilometer: $";
    cin >> feePerKm;
    cin.ignore();

    // Escape special characters
    char* escapedVehicle = new char[vehicleNumber.length() * 2 + 1];
    char* escapedPhone = new char[phoneNumber.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedVehicle, vehicleNumber.c_str(), vehicleNumber.length());
    mysql_real_escape_string(conn, escapedPhone, phoneNumber.c_str(), phoneNumber.length());

    string query = "INSERT INTO HospitalAmbulances (hospital_id, vehicle_number, phone_number, fee_per_km) "
            "VALUES (" + to_string(hospital->getUserId()) + ", '" + escapedVehicle + "', '" + 
            escapedPhone + "', " + to_string(feePerKm) + ")";

    delete[] escapedVehicle;
    delete[] escapedPhone;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Ambulance added successfully!\n";
    } else {
        cout << "Error adding ambulance: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::removeAmbulance() {
    cout << "\n=== Remove Ambulance ===\n";
    showAmbulances();

    string vehicleNumber;
    cout << "\nEnter Vehicle Number to remove: ";
    getline(cin, vehicleNumber);

    // Escape special characters
    char* escapedVehicle = new char[vehicleNumber.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedVehicle, vehicleNumber.c_str(), vehicleNumber.length());

    string query = "DELETE FROM HospitalAmbulances WHERE hospital_id = " + to_string(hospital->getUserId()) +
                  " AND vehicle_number = '" + escapedVehicle + "'";

    delete[] escapedVehicle;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Ambulance removed successfully!\n";
    } else {
        cout << "Error removing ambulance: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::addMedicalService() {
    cout << "\n=== Add Medical Service ===\n";
    cout << "1. Add Test\n";
    cout << "2. Add Medicine\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string name;
    cout << "Enter Name: ";
    getline(cin, name);

    double price;
    cout << "Enter Price: $";
    cin >> price;
    cin.ignore();

    // Escape special characters
    char* escapedName = new char[name.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedName, name.c_str(), name.length());

    string query;
    if (choice == 1) {
        query = "INSERT INTO Tests (hospital_id, name, price) "
                "VALUES (" + to_string(hospital->getUserId()) + ", '" + escapedName + "', " + to_string(price) + ")";
    } else if (choice == 2) {
        query = "INSERT INTO Medicines (hospital_id, name, price) "
                "VALUES (" + to_string(hospital->getUserId()) + ", '" + escapedName + "', " + to_string(price) + ")";
    } else {
        cout << "Invalid choice!\n";
        delete[] escapedName;
        return;
    }

    delete[] escapedName;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Medical service added successfully!\n";
    } else {
        cout << "Error adding medical service: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::removeMedicalService() {
    cout << "\n=== Remove Medical Service ===\n";
    cout << "1. Remove Test\n";
    cout << "2. Remove Medicine\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string name;
    cout << "Enter Name to remove: ";
    getline(cin, name);

    // Escape special characters
    char* escapedName = new char[name.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedName, name.c_str(), name.length());

    string query;
    if (choice == 1) {
        query = "DELETE FROM Tests WHERE hospital_id = " + to_string(hospital->getUserId()) +
                " AND name = '" + escapedName + "'";
    } else if (choice == 2) {
        query = "DELETE FROM Medicines WHERE hospital_id = " + to_string(hospital->getUserId()) +
                " AND name = '" + escapedName + "'";
    } else {
        cout << "Invalid choice!\n";
        delete[] escapedName;
        return;
    }

    delete[] escapedName;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Medical service removed successfully!\n";
    } else {
        cout << "Error removing medical service: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::updateDepartmentFees() {
    cout << "\n=== Update Department Fees ===\n";
    showDepartments();

    int departmentId;
    cout << "\nEnter Department ID: ";
    cin >> departmentId;
    cin.ignore();

    if (!validateDepartment(departmentId)) {
        cout << "Invalid department ID.\n";
        return;
    }

    double newFee;
    cout << "Enter New Appointment Fee: $";
    cin >> newFee;
    cin.ignore();

    string query = "UPDATE HospitalDepartments SET appointment_fee = " + to_string(newFee) +
                  " WHERE hospital_id = " + to_string(hospital->getUserId()) +
                  " AND department_id = " + to_string(departmentId);

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Department fees updated successfully!\n";
    } else {
        cout << "Error updating department fees: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::updateSurgeryFees() {
    cout << "\n=== Update Surgery Fees ===\n";
    
    string query = "SELECT s.surgery_id, s.name, hsf.fee "
                  "FROM Surgeries s "
                  "LEFT JOIN HospitalSurgeryFees hsf ON s.surgery_id = hsf.surgery_id "
                  "AND hsf.hospital_id = " + to_string(hospital->getUserId());
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching surgeries: " << mysql_error(conn) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << row[0] << ". " << row[1] << " - Current Fee: $" << (row[2] ? row[2] : "Not Set") << endl;
    }
    mysql_free_result(result);

    int surgeryId;
    cout << "\nEnter Surgery ID: ";
    cin >> surgeryId;
    cin.ignore();

    if (!validateSurgery(surgeryId)) {
        cout << "Invalid surgery ID.\n";
        return;
    }

    double newFee;
    cout << "Enter New Surgery Fee: $";
    cin >> newFee;
    cin.ignore();

    query = "INSERT INTO HospitalSurgeryFees (hospital_id, surgery_id, fee) "
            "VALUES (" + to_string(hospital->getUserId()) + ", " + to_string(surgeryId) + ", " + to_string(newFee) + ") "
            "ON DUPLICATE KEY UPDATE fee = " + to_string(newFee);

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Surgery fees updated successfully!\n";
    } else {
        cout << "Error updating surgery fees: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::updateAppointmentFees() {
    cout << "\n=== Update Appointment Fees ===\n";
    showDoctors();

    int doctorId;
    cout << "\nEnter Doctor ID: ";
    cin >> doctorId;
    cin.ignore();

    if (!validateDoctor(doctorId)) {
        cout << "Invalid doctor ID.\n";
        return;
    }

    double newFee;
    cout << "Enter New Appointment Fee: $";
    cin >> newFee;
    cin.ignore();

    string query = "UPDATE DoctorAppointments SET price = " + to_string(newFee) +
                  " WHERE hospital_id = " + to_string(hospital->getUserId()) +
                  " AND doctor_id = " + to_string(doctorId);

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Appointment fees updated successfully!\n";
    } else {
        cout << "Error updating appointment fees: " << mysql_error(conn) << "\n";
    }
}

void HospitalDashboard::viewAppointments() const {
    cout << "\n=== Hospital Appointments ===\n";
    
    string query = "SELECT a.appointment_id, u.name as patient_name, d.name as doctor_name, "
                  "a.appointment_date, a.appointment_time, a.status "
                  "FROM Appointments a "
                  "JOIN Patients p ON a.patient_id = p.patient_id "
                  "JOIN Users u ON p.user_id = u.user_id "
                  "JOIN Doctors d ON a.doctor_id = d.doctor_id "
                  "JOIN DoctorHospitalAffiliations dha ON d.doctor_id = dha.doctor_id "
                  "WHERE dha.hospital_id = " + to_string(hospital->getUserId()) +
                  " ORDER BY a.appointment_date, a.appointment_time";

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching appointments: " << mysql_error(conn) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No appointments found.\n";
        mysql_free_result(result);
        return;
    }

    cout << left << setw(10) << "ID" << setw(25) << "Patient" << setw(25) << "Doctor"
         << setw(15) << "Date" << setw(15) << "Time" << "Status" << endl;
    cout << string(100, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(10) << row[0] << setw(25) << row[1] << setw(25) << row[2]
             << setw(15) << row[3] << setw(15) << row[4] << row[5] << endl;
    }

    mysql_free_result(result);
}

void HospitalDashboard::viewBills() const {
    cout << "\n=== Hospital Bills ===\n";
    
    string query = "SELECT b.bill_id, u.name as patient_name, b.amount, b.description, "
                  "b.issue_date, b.due_date, b.status "
                  "FROM Bills b "
                  "JOIN Patients p ON b.patient_id = p.patient_id "
                  "JOIN Users u ON p.user_id = u.user_id "
                  "WHERE b.hospital_id = " + to_string(hospital->getUserId()) +
                  " ORDER BY b.issue_date DESC";

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching bills: " << mysql_error(conn) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No bills found.\n";
        mysql_free_result(result);
        return;
    }

    cout << left << setw(10) << "ID" << setw(25) << "Patient" << setw(15) << "Amount"
         << setw(30) << "Description" << setw(15) << "Issue Date" << setw(15) << "Due Date"
         << "Status" << endl;
    cout << string(120, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(10) << row[0] << setw(25) << row[1] << "$" << setw(14) << row[2]
             << setw(30) << row[3] << setw(15) << row[4] << setw(15) << row[5] << row[6] << endl;
    }

    mysql_free_result(result);
}

void HospitalDashboard::viewPayments() const {
    cout << "\n=== Hospital Payments ===\n";
    
    string query = "SELECT p.payment_id, u.name as patient_name, b.amount, p.payment_date, "
                  "p.payment_method "
                  "FROM Payments p "
                  "JOIN Bills b ON p.bill_id = b.bill_id "
                  "JOIN Patients pt ON b.patient_id = pt.patient_id "
                  "JOIN Users u ON pt.user_id = u.user_id "
                  "WHERE b.hospital_id = " + to_string(hospital->getUserId()) +
                  " ORDER BY p.payment_date DESC";

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error fetching payments: " << mysql_error(conn) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        cout << "No payments found.\n";
        mysql_free_result(result);
        return;
    }

    cout << left << setw(10) << "ID" << setw(25) << "Patient" << setw(15) << "Amount"
         << setw(20) << "Payment Date" << "Method" << endl;
    cout << string(80, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(10) << row[0] << setw(25) << row[1] << "$" << setw(14) << row[2]
             << setw(20) << row[3] << row[4] << endl;
    }

    mysql_free_result(result);
}

void HospitalDashboard::generateReport() {
    cout << "\n=== Generate Report ===\n";
    cout << "1. Billing Report\n";
    cout << "2. Patient Statistics\n";
    cout << "3. Hospital Performance\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string reportType;
    string data;

    switch (choice) {
        case 1: {
            reportType = "Billing";
            string query = "SELECT COUNT(*) as total_bills, "
                          "SUM(CASE WHEN status = 'Paid' THEN 1 ELSE 0 END) as paid_bills, "
                          "SUM(CASE WHEN status = 'Pending' THEN 1 ELSE 0 END) as pending_bills, "
                          "SUM(CASE WHEN status = 'Overdue' THEN 1 ELSE 0 END) as overdue_bills, "
                          "SUM(amount) as total_amount "
                          "FROM Bills "
                          "WHERE hospital_id = " + to_string(hospital->getUserId());

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error generating billing report: " << mysql_error(conn) << "\n";
                return;
            }

            MYSQL_RES* result = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(result);
            
            data = "Total Bills: " + string(row[0]) + "\n" +
                   "Paid Bills: " + string(row[1]) + "\n" +
                   "Pending Bills: " + string(row[2]) + "\n" +
                   "Overdue Bills: " + string(row[3]) + "\n" +
                   "Total Amount: $" + string(row[4]) + "\n";

            mysql_free_result(result);
            break;
        }
        case 2: {
            reportType = "Patient Statistics";
            string query = "SELECT COUNT(DISTINCT a.patient_id) as total_patients, "
                          "COUNT(*) as total_appointments, "
                          "AVG(DATEDIFF(a.appointment_date, p.dob)/365) as avg_patient_age "
                          "FROM Appointments a "
                          "JOIN Patients p ON a.patient_id = p.patient_id "
                          "JOIN Doctors d ON a.doctor_id = d.doctor_id "
                          "JOIN DoctorHospitalAffiliations dha ON d.doctor_id = dha.doctor_id "
                          "WHERE dha.hospital_id = " + to_string(hospital->getUserId());

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error generating patient statistics: " << mysql_error(conn) << "\n";
                return;
            }

            MYSQL_RES* result = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(result);
            
            data = "Total Unique Patients: " + string(row[0]) + "\n" +
                   "Total Appointments: " + string(row[1]) + "\n" +
                   "Average Patient Age: " + string(row[2]) + " years\n";

            mysql_free_result(result);
            break;
        }
        case 3: {
            reportType = "Hospital Performance";
            string query = "SELECT COUNT(*) as total_appointments, "
                          "COUNT(CASE WHEN status = 'completed' THEN 1 END) as completed_appointments, "
                          "AVG(rating) as avg_rating, "
                          "COUNT(rating) as total_ratings "
                          "FROM Appointments a "
                          "LEFT JOIN HospitalReviews hr ON hr.hospital_id = " + to_string(hospital->getUserId()) + " "
                          "JOIN Doctors d ON a.doctor_id = d.doctor_id "
                          "JOIN DoctorHospitalAffiliations dha ON d.doctor_id = dha.doctor_id "
                          "WHERE dha.hospital_id = " + to_string(hospital->getUserId());

            if (mysql_query(conn, query.c_str()) != 0) {
                cout << "Error generating performance report: " << mysql_error(conn) << "\n";
                return;
            }

            MYSQL_RES* result = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(result);
            
            data = "Total Appointments: " + string(row[0]) + "\n" +
                   "Completed Appointments: " + string(row[1]) + "\n" +
                   "Completion Rate: " + to_string((stoi(row[1]) * 100.0) / stoi(row[0])) + "%\n" +
                   "Average Rating: " + string(row[2]) + "\n" +
                   "Total Ratings: " + string(row[3]) + "\n";

            mysql_free_result(result);
            break;
        }
        default:
            cout << "Invalid choice!\n";
            return;
    }

    // Insert report into database
    char* escapedData = new char[data.length() * 2 + 1];
    mysql_real_escape_string(conn, escapedData, data.c_str(), data.length());

    string query = "INSERT INTO Reports (report_type, data) VALUES ('" + reportType + "', '" + escapedData + "')";
    delete[] escapedData;

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Report generated successfully!\n";
        cout << "\nReport Data:\n" << data;
    } else {
        cout << "Error generating report: " << mysql_error(conn) << "\n";
    }
}

bool HospitalDashboard::validateDepartment(int departmentId) {
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
        cout << "Invalid department ID.\n";
    }
    
    return exists;
}

bool HospitalDashboard::validateDoctor(int doctorId) {
    string query = "SELECT COUNT(*) FROM Doctors WHERE doctor_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error validating doctor: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = stoi(row[0]) > 0;
    mysql_free_result(result);
    
    if (!exists) {
        cout << "Invalid doctor ID.\n";
    }
    
    return exists;
}

bool HospitalDashboard::validateSurgery(int surgeryId) {
    string query = "SELECT COUNT(*) FROM Surgeries WHERE surgery_id = " + to_string(surgeryId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error validating surgery: " << mysql_error(conn) << "\n";
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = stoi(row[0]) > 0;
    mysql_free_result(result);
    
    if (!exists) {
        cout << "Invalid surgery ID.\n";
    }
    
    return exists;
}

bool HospitalDashboard::validateAmbulanceDetails(const string& vehicleNumber, const string& phoneNumber) {
    if (vehicleNumber.empty() || phoneNumber.empty()) {
        cout << "Vehicle number and phone number cannot be empty.\n";
        return false;
    }

    if (phoneNumber.length() > 15) {
        cout << "Phone number is too long.\n";
        return false;
    }

    return true;
}

string HospitalDashboard::getDepartmentName(int departmentId) {
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

string HospitalDashboard::getDoctorName(int doctorId) {
    string query = "SELECT u.name FROM Users u JOIN Doctors d ON u.user_id = d.user_id "
                  "WHERE d.doctor_id = " + to_string(doctorId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        return "Unknown Doctor";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    string name = row ? row[0] : "Unknown Doctor";
    mysql_free_result(result);
    return name;
}

string HospitalDashboard::getSurgeryName(int surgeryId) {
    string query = "SELECT name FROM Surgeries WHERE surgery_id = " + to_string(surgeryId);
    
    if (mysql_query(conn, query.c_str()) != 0) {
        return "Unknown Surgery";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    string name = row ? row[0] : "Unknown Surgery";
    mysql_free_result(result);
    return name;
} 