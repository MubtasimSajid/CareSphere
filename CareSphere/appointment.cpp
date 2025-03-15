#include <sstream>  // Required for std::stringstream
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

string Get_User_Appointments(const string &user_name)
{
    QSqlQuery query;
    query.prepare("SELECT appointment_date, appointment_time, doctor_name, location FROM appointments WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<Appointment> appointments; // Store all appointments

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return "";
    }

    // Store results in vector
    while (query.next()) {
        appointments.emplace_back(
            user_name,  // Fix: Added user_name to match constructor
            query.value(2).toString().toUtf8().constData(),  // Doctor Name
            query.value(3).toString().toUtf8().constData(),  // Location
            query.value(0).toString().toUtf8().constData(),  // Date
            query.value(1).toString().toUtf8().constData()   // Time
            );
    }

    if (appointments.empty()) {
        return "";
    }

    // Format the output string with bullet points (•)
    stringstream result;

    for (const auto &a : appointments) {
        result << ". " << a.getDate() << " - " << a.getTime()
            << " with Dr. " << a.getDoctorName()
            << " at " << a.getLocation() << "\n";
    }

    return result.str(); // Return the formatted appointment list
}

