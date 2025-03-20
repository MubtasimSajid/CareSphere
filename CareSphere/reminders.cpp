#include "reminders.h"

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
    return "• " + title + " - " + date + " - " + time + " - " + note;
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
vector<string> Get_User_Reminders(const string &user_name) {
    QSqlQuery query;
    query.prepare("SELECT title, reminder_date, reminder_time, note FROM reminders WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(user_name));

    vector<string> remindersList; // Store each reminder as a formatted string

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return {};
    }

    // Store results in vector
    while (query.next()) {
        string reminderEntry = string(query.value(0).toString().toUtf8().constData()) +  // Title
                               " - " + string(query.value(1).toString().toUtf8().constData());  // Date

        // Append time if available
        string time = query.value(2).toString().toUtf8().constData();
        if (!time.empty()) {
            reminderEntry += " - " + time;
        }

        // Append note if available
        string note = query.value(3).toString().toUtf8().constData();
        if (!note.empty()) {
            reminderEntry += " - " + note;
        }

        remindersList.push_back(reminderEntry);
    }

    return remindersList; // Return vector of formatted reminder strings
}



void Update_Reminder(string user_name, string newTitle, string newReminderDate, string newReminderTime, string newNote,
                     string oldTitle, string oldReminderDate, string oldReminderTime, string oldNote)
{
    QSqlQuery query;

    // Correct SQL query syntax
    query.prepare("UPDATE reminders SET title = :newTitle, reminder_date = :newReminderDate, reminder_time = :newReminderTime, note = :newNote "
                  "WHERE user_id = :userId AND title = :oldTitle AND reminder_date = :oldReminderDate AND reminder_time = :oldReminderTime AND note = :oldNote");

    // Bind values to the query parameters
    query.bindValue(":newTitle", QString::fromStdString(newTitle));
    query.bindValue(":newReminderDate", QString::fromStdString(newReminderDate));
    query.bindValue(":newReminderTime", QString::fromStdString(newReminderTime));
    query.bindValue(":newNote", QString::fromStdString(newNote));
    query.bindValue(":userId", QString::fromStdString(user_name));  // Assuming user_name is passed as argument
    query.bindValue(":oldTitle", QString::fromStdString(oldTitle));
    query.bindValue(":oldReminderDate", QString::fromStdString(oldReminderDate));
    query.bindValue(":oldReminderTime", QString::fromStdString(oldReminderTime));
    query.bindValue(":oldNote", QString::fromStdString(oldNote));

    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError().text();
    } else {
        qDebug() << "Reminder updated successfully!";
    }
}


void Delete_Reminder(string user_name, string Title, string ReminderDate, string ReminderTime, string note)
{
    QSqlQuery query;

    query.prepare("DELETE FROM reminders "
                  "WHERE user_id = :userId AND title = :Title AND reminder_date = :ReminderDate AND reminder_time = :ReminderTime AND note = :Note");

    query.bindValue(":userId", QString::fromStdString(user_name));
    query.bindValue(":Title", QString::fromStdString(Title));
    query.bindValue(":ReminderDate", QString::fromStdString(ReminderDate));
    query.bindValue(":ReminderTime", QString::fromStdString(ReminderTime));
    query.bindValue(":Note", QString::fromStdString(note));

    if (!query.exec()) {
        qDebug() << "Deletion failed: " << query.lastError().text();
    } else {
        if (query.numRowsAffected() > 0) {
            qDebug() << "Reminder deleted successfully!";
        } else {
            qDebug() << "No reminder found with the provided details.";
        }
    }
}

