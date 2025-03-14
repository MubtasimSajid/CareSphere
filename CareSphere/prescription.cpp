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
// Default Constructor
Prescription::Prescription() {
    user_name = "";
    medicine = "";
    dosage = "";
    frequency = "";
}

// Parameterized Constructor
Prescription::Prescription(string un, string m, string d, string f) {
    user_name = un;
    medicine = m;
    dosage = d;
    frequency = f;
}

// Setters
void Prescription::setUserName(string un) {
    user_name = un;
}

void Prescription::setMedicine(string m) {
    medicine = m;
}

void Prescription::setDosage(string d) {
    dosage = d;
}

void Prescription::setFrequency(string f) {
    frequency = f;
}

// Getters
string Prescription::getUserName() const {
    return user_name;
}

string Prescription::getMedicine() const {
    return medicine;
}

string Prescription::getDosage() const {
    return dosage;
}

string Prescription::getFrequency() const {
    return frequency;
}


// Display Details Function
void Prescription::displayDetails() const {
    cout << "Prescription Details:\n";
    cout << "User: " << user_name << "\n";
    cout << "Medicine: " << medicine << "\n";
    cout << "Dosage: " << dosage << "\n";
    cout << "Frequency: " << frequency << "\n";
}



void SavePrescription(const Prescription &p)
{
    QSqlQuery query;
    QString insert_user = "INSERT INTO prescriptions (user_id, medicine, dosage, frequency) VALUES ('"
                          + QString::fromStdString(p.getUserName()) + "', '"
                          + QString::fromStdString(p.getMedicine()) + "', '"
                          + QString::fromStdString(p.getDosage()) + "', '"
                          + QString::fromStdString(p.getFrequency()) + "' )";

    MySQL_Insert(insert_user);
}



string GetUserPrescriptions(const string &user_name) {
    QSqlQuery query;
    query.prepare("SELECT user_id, medicine, dosage, frequency FROM prescriptions WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<Prescription> prescriptions; // Store all prescriptions

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return "Error retrieving prescriptions.";
    }

    // Store results in vector
    while (query.next()) {
        prescriptions.emplace_back(
            query.value(0).toString().toStdString(), // user_name
            query.value(1).toString().toStdString(), // medicine
            query.value(2).toString().toStdString(), // dosage
            query.value(3).toString().toStdString()  // frequency
            );
    }

    if (prescriptions.empty()) {
        return "No prescriptions found for user: " + user_name;
    }

    // Define column widths
    int col1_width = 15; // Medicine
    int col2_width = 10; // Dosage
    int col3_width = 25; // Frequency

    // Convert vector contents to a properly aligned table
    stringstream result;
    result << "Prescriptions for " << user_name << ":\n\n";

    // Print table header
    result << "+" << string(col1_width, '-') << "+" << string(col2_width, '-') << "+" << string(col3_width, '-') << "+\n";
    result << "| " << left << setw(col1_width - 1) << "Medicine"
           << "| " << setw(col2_width - 1) << "Dosage"
           << "| " << setw(col3_width - 1) << "Frequency"
           << "|\n";
    result << "+" << string(col1_width, '-') << "+" << string(col2_width, '-') << "+" << string(col3_width, '-') << "+\n";

    // Print each row
    for (const auto &p : prescriptions) {
        result << "| " << left << setw(col1_width - 1) << p.getMedicine()
        << "| " << setw(col2_width - 1) << p.getDosage()
        << "| " << setw(col3_width - 1) << p.getFrequency()
        << "|\n";
    }

    // Print bottom border
    result << "+" << string(col1_width, '-') << "+" << string(col2_width, '-') << "+" << string(col3_width, '-') << "+\n";

    return result.str(); // Return the formatted string
}

