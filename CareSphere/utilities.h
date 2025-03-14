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

inline std::string get_User_Notes(const std::string &username) {
    QSqlQuery query;

    // Query to get all notes for the given user_id
    query.prepare("SELECT notes FROM NOTES WHERE user_id = :username");
    query.bindValue(":username", QString::fromStdString(username));

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return "";  // Return an empty string on failure
    }

    std::string all_notes;
    while (query.next()) {
        if (!all_notes.empty()) {
            all_notes += "\n";  // Add newline separator between notes
        }
        all_notes += query.value(0).toString().toStdString();
    }

    return all_notes.empty() ? "No notes found for this user." : all_notes;
}

#endif
