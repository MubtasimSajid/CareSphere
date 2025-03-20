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



vector<string> GetUserPrescriptions(const string &user_name) {
    QSqlQuery query;
    query.prepare("SELECT doctor_name, MEDICINES FROM prescriptions WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<string> prescriptionList; // Store all prescription strings

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return {};
    }

    // Store results in vector
    while (query.next()) {
        string doctorName = query.value(0).toString().toStdString();
        string medicineNotes = query.value(1).toString().toStdString();
        string prescriptionEntry = "Dr. " + doctorName + ": " + medicineNotes;

        prescriptionList.push_back(prescriptionEntry);
    }

    return prescriptionList; // Return vector of prescription strings
}





void UpdatePrescription(const string& user_name,
                        const string& doctor__Name_new,
                        const string& medicine_Text_new,
                        const string& doctor__Name_old,
                        const string& medicine_Text_old)
{
    QSqlQuery query;

    query.prepare("UPDATE prescriptions SET doctor_name = :newDoctor, MEDICINES = :newMedicines "
                  "WHERE user_id = :userId AND doctor_name = :oldDoctor AND MEDICINES = :oldMedicines");

    query.bindValue(":newDoctor", QString::fromStdString(doctor__Name_new));
    query.bindValue(":newMedicines", QString::fromStdString(medicine_Text_new));
    query.bindValue(":userId", QString::fromStdString(user_name));
    query.bindValue(":oldDoctor", QString::fromStdString(doctor__Name_old));
    query.bindValue(":oldMedicines", QString::fromStdString(medicine_Text_old));

    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError().text();
    } else {
        qDebug() << "Prescription updated successfully!";
    }
}

