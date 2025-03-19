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

// Function to encrypt the password
inline string encryptPassword(const string& password) {
    string encrypted = password;
    for (char &c : encrypted) {
        c ^= SECRET_KEY;  // XOR each character with the key
    }
    return encrypted;
}

// Function to decrypt the password (XOR is reversible)
inline string decryptPassword(const string& encryptedPassword) {
    return encryptPassword(encryptedPassword);  // XOR again to get the original password
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

inline void Delete_User_Notes(const string &user_name, const string &notes){
    QSqlQuery query;
    QString update = "DELETE FROM NOTES WHERE notes = '"+
                     QString::fromStdString(notes) +
                     "' and user_id = '" +
                     QString::fromStdString(user_name) + "' ";

    MySQL_Update(update);
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

    return notes.empty() ? std::vector<std::string>{"No notes found for this user."} : notes;
}

#endif
