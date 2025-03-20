#include <sstream>  // Required for std::stringstream
#include<string>
#include<vector>
#include "appointment.h"
#include "MySQL_utilities.h"
// Default constructor
Appointment::Appointment() {
    user_name = "";
    doctor_name = "";
    location = "";
    date = "";
    time = "";
}

// Parameterized constructor
Appointment::Appointment(string un, string dn, string loc, string d, string t) {
    user_name = un;
    doctor_name = dn;
    location = loc;
    date = d;
    time = t;
}

// Setters
void Appointment::setUserName(string un) { user_name = un; }
void Appointment::setDoctorName(string dn) { doctor_name = dn; }
void Appointment::setLocation(string loc) { location = loc; }
void Appointment::setDate(string d) { date = d; }
void Appointment::setTime(string t) { time = t; }

// Getters
string Appointment::getUserName() const { return user_name; }
string Appointment::getDoctorName() const { return doctor_name; }
string Appointment::getLocation() const { return location; }
string Appointment::getDate() const { return date; }
string Appointment::getTime() const { return time; }

// Function to return formatted appointment output
string Appointment::formatAppointment() const {
    return ". " + date + " - " + time + " with Dr. " + doctor_name + " at " + location;
}

void Save_User_Appointment(Appointment a)
{
    QSqlQuery query;
    QString insert_user = "INSERT INTO appointments (user_id, doctor_name, location, appointment_date, appointment_time) VALUES ('"
                          + QString::fromStdString(a.getUserName()) + "', '"
                          + QString::fromStdString(a.getDoctorName()) + "', '"
                          + QString::fromStdString(a.getLocation()) + "', '"
                          + QString::fromStdString(a.getDate()) + "', '"
                          + QString::fromStdString(a.getTime()) + "' )";

    MySQL_Insert(insert_user);
}

vector<string> Get_User_Appointments(const string &user_name)
{
    QSqlQuery query;
    query.prepare("SELECT appointment_date, appointment_time, doctor_name, location FROM appointments WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<string> appointmentsList; // Vector to store formatted appointment strings

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return {};
    }

    // Store results in vector
    while (query.next()) {
        // Convert QString values to std::string
        string date = query.value(0).toString().toStdString();
        string time = query.value(1).toString().toStdString();
        string doctorName = query.value(2).toString().toStdString();
        string location = query.value(3).toString().toStdString();

        // Use std::string concatenation
        string appointmentEntry = ". " + date +
                                  " - " + time +
                                  " with Dr. " + doctorName +
                                  " at " + location;

        appointmentsList.push_back(appointmentEntry);
    }

    return appointmentsList; // Return vector of formatted appointment strings
}



void Update_User_Appointment(string user_name, string newDoctorName, string newLocation, string newAppointmentDate, string newAppointmentTime,
                             string oldDoctorName, string oldLocation, string oldAppointmentDate, string oldAppointmentTime)
{
    QSqlQuery query;


    query.prepare("UPDATE appointments SET doctor_name = :newDoctorName, location = :newLocation, "
                  "appointment_date = :newAppointmentDate, appointment_time = :newAppointmentTime "
                  "WHERE user_id = :userId AND doctor_name = :oldDoctorName AND location = :oldLocation "
                  "AND appointment_date = :oldAppointmentDate AND appointment_time = :oldAppointmentTime");


    query.bindValue(":newDoctorName", QString::fromStdString(newDoctorName));
    query.bindValue(":newLocation", QString::fromStdString(newLocation));  // ✅ Fixed Typo
    query.bindValue(":newAppointmentDate", QString::fromStdString(newAppointmentDate));
    query.bindValue(":newAppointmentTime", QString::fromStdString(newAppointmentTime));
    query.bindValue(":userId", QString::fromStdString(user_name));
    query.bindValue(":oldDoctorName", QString::fromStdString(oldDoctorName));
    query.bindValue(":oldLocation", QString::fromStdString(oldLocation));
    query.bindValue(":oldAppointmentDate", QString::fromStdString(oldAppointmentDate));
    query.bindValue(":oldAppointmentTime", QString::fromStdString(oldAppointmentTime));


    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError().text();
    } else if (query.numRowsAffected() == 0) {
        qDebug() << "No matching appointment found to update.";
    } else {
        qDebug() << "Appointment updated successfully!";
    }
}


void Delete_User_Appointment(string user_name, string DoctorName, string Location, string AppointmentDate, string AppointmentTime)
{
    QSqlQuery query;

    query.prepare("DELETE FROM appointments "
                  "WHERE user_id = :userId AND doctor_name = :DoctorName AND location = :Location AND appointment_date = :AppointmentDate AND appointment_time = :AppointmentTime");

    query.bindValue(":userId", QString::fromStdString(user_name));
    query.bindValue(":DoctorName", QString::fromStdString(DoctorName));
    query.bindValue(":Location", QString::fromStdString(Location));
    query.bindValue(":AppointmentDate", QString::fromStdString(AppointmentDate));
    query.bindValue(":AppointmentTime", QString::fromStdString(AppointmentTime));

    if (!query.exec()) {
        qDebug() << "Deletion failed: " << query.lastError().text();
    } else {
        if (query.numRowsAffected() > 0) {
            qDebug() << "Appointment deleted successfully!";
        } else {
            qDebug() << "No appointment found with the provided details.";
        }
    }
}
