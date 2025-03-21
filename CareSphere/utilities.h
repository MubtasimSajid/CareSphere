#ifndef UTILITIES_H
#define UTILITIES_H

#include "user.h"
#include "MySQL_utilities.h"
#include <string>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

using namespace std;

const char SECRET_KEY = 'K';

inline string encryptPassword(const string& password) {
    string encrypted = password;
    for (char &c : encrypted) {
        c ^= SECRET_KEY;
    }
    return encrypted;
}

inline string decryptPassword(const string& encryptedPassword) {
    return encryptPassword(encryptedPassword);
}

inline void save_User_Notes(const string &user_name, const string &notes){
    QSqlQuery query;
    QString insert_user = "INSERT INTO NOTES (user_id, notes) VALUES ('"
                          + QString::fromStdString(user_name) + "', '"
                          + QString::fromStdString(notes) + "' )";

    MySQL_Insert(insert_user);
}

inline void Update_User_Notes(const string &user_name, const string &notes, const string &orginal_note){
    QSqlQuery query;
    QString update = "UPDATE NOTES SET notes = '"+
                     QString::fromStdString(notes) +
                     "' WHERE user_id = '" +
                     QString::fromStdString(user_name) +
                     "' and notes = '" +
                     QString::fromStdString(orginal_note) + "' ";

    MySQL_Update(update);
}

inline void Delete_User_Notes(const string &user_name, const string &notes) {
    QSqlQuery query;
    query.prepare("DELETE FROM NOTES WHERE notes = :notes AND user_id = :user_name");
    query.bindValue(":notes", QString::fromStdString(notes));
    query.bindValue(":user_name", QString::fromStdString(user_name));

    if (!query.exec()) {
        qDebug() << "Error deleting note: " << query.lastError().text();
    }
}


inline std::vector<std::string> get_User_Notes(const std::string &username) {
    QSqlQuery query;

    // Query to get all notes for the given user_id
    query.prepare("SELECT notes FROM NOTES WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(username));

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return {};  // Return an empty vector on failure
    }

    std::vector<std::string> notes;
    while (query.next()) {
        notes.push_back(query.value(0).toString().toStdString());  // Add each note as a separate string
    }

    return notes.empty() ? std::vector<std::string>{""} : notes;
}

#endif
