#include "Prescription.h"
#include "MySQL_utilities.h"
#include"utilities.h"
#include <QDebug>
#include <fstream>
#include <vector>
#include <sstream>   // String stream for CSV parsing
#include <sys/stat.h> // For creating directories
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <iomanip> // For table formatting
// Default constructor
Prescription::Prescription() {
    user_name = "";
    doctor_name = "";
    medicines = "";
}

// Parameterized constructor
Prescription::Prescription(string un, string dn, string m) {
    user_name = un;
    doctor_name = dn;
    medicines = m;
}

// Setters
void Prescription::setUserName(string un) {
    user_name = un;
}

void Prescription::setDoctorName(string dn) {
    doctor_name = dn;
}

void Prescription::setMedicineNotes(string m) {
    medicines = m;
}

// Getters
string Prescription::getUserName() const {
    return user_name;
}

string Prescription::getDoctorName() const {
    return doctor_name;
}

string Prescription::getMedicineNotes() const {
    return medicines;
}

// Display prescription details
void Prescription::displayDetails() const {
    cout << "User Name: " << user_name << endl;
    cout << "Doctor Name: " << doctor_name << endl;
    cout << "Medicines: " << medicines << endl;
}



void SavePrescription(const Prescription &p)
{
    QSqlQuery query;
    QString insert_user = "INSERT INTO prescriptions (user_id, doctor_name, MEDICINES) VALUES ('"
                          + QString::fromStdString(p.getUserName()) + "', '"
                          + QString::fromStdString(p.getDoctorName()) + "', '"
                          + QString::fromStdString(p.getMedicineNotes()) + "' )";

    MySQL_Insert(insert_user);
}



string GetUserPrescriptions(const string &user_name) {
    QSqlQuery query;
    query.prepare("SELECT doctor_name, MEDICINES FROM prescriptions WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<Prescription> prescriptions; // Store all prescriptions

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return "";
    }

    // Store results in vector
    while (query.next()) {
        prescriptions.emplace_back(
            user_name,
            query.value(0).toString().toUtf8().constData(),  // Use UTF-8 safe conversion
            query.value(1).toString().toUtf8().constData()
            );
    }

    if (prescriptions.empty()) {
        return "";
    }

    // Format the output string with properly encoded bullet points
    stringstream result;
    result << "Prescriptions for " << user_name << ":\n";

    for (const auto &p : prescriptions) {
        result << ".Dr. " << p.getDoctorName() << ": " << p.getMedicineNotes() << "\n";
    }

    return result.str(); // Return the formatted prescription list
}



