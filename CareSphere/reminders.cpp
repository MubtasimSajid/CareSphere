#include "reminders.h"
#include "MySQL_utilities.h"
#include <sstream>  // Ensure you include this for stringstream
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Default constructor
Reminder::Reminder() {
    user_name = "";
    title = "";
    date = "";
    time = "";
    note = "";
}

// Parameterized constructor
Reminder::Reminder(string un, string t, string d, string tm, string n) {
    user_name = un;
    title = t;
    date = d;
    time = tm;
    note = n;
}

// Setters
void Reminder::setUserName(string un) { user_name = un; }
void Reminder::setTitle(string t) { title = t; }
void Reminder::setDate(string d) { date = d; }
void Reminder::setTime(string tm) { time = tm; }
void Reminder::setNote(string n) { note = n; }

// Getters
string Reminder::getUserName() const { return user_name; }
string Reminder::getTitle() const { return title; }
string Reminder::getDate() const { return date; }
string Reminder::getTime() const { return time; }
string Reminder::getNote() const { return note; }

// Function to return formatted reminder output
string Reminder::formatReminder() const {
    return "• " + title + " - " + date + " at " + time + " - " + note;
}

// Function to Save a Reminder
void Save_User_Reminder(Reminder r) {
    QSqlQuery query;
    QString insert_user = "INSERT INTO reminders (user_id, title, reminder_date, reminder_time, note) VALUES ('"
                          + QString::fromStdString(r.getUserName()) + "', '"
                          + QString::fromStdString(r.getTitle()) + "', '"
                          + QString::fromStdString(r.getDate()) + "', '"
                          + QString::fromStdString(r.getTime()) + "', '"
                          + QString::fromStdString(r.getNote()) + "' )";

    MySQL_Insert(insert_user);
}

// Function to Retrieve User Reminders
string Get_User_Reminders(const string &user_name) {
    QSqlQuery query;
    query.prepare("SELECT title, reminder_date, reminder_time, note FROM reminders WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<Reminder> reminders; // Store all reminders

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return "";
    }

    // Store results in vector
    while (query.next()) {
        reminders.emplace_back(
            user_name,  // Fix: Added user_name to match constructor
            query.value(0).toString().toUtf8().constData(),  // Title
            query.value(1).toString().toUtf8().constData(),  // Date
            query.value(2).toString().isEmpty() ? "N/A" : query.value(2).toString().toUtf8().constData(), // Time (Handle NULL)
            query.value(3).toString().isEmpty() ? "" : query.value(3).toString().toUtf8().constData() // Note (Handle NULL)
            );
    }

    if (reminders.empty()) {
        return "";
    }

    // Format the output string with bullet points (•)
    stringstream result;

    for (const auto &r : reminders) {
        result << ". " << r.getTitle() << " - " << r.getDate();

        // Append time if available
        if (r.getTime() != "N/A") {
            result << " at " << r.getTime();
        }

        // Append note if available
        if (!r.getNote().empty()) {
            result << " - " << r.getNote();
        }

        result << "\n"; // Newline for next reminder
    }

    return result.str(); // Return the formatted reminder list
}
