#ifndef UTILITIES_H
#define UTILITIES_H

#include "user.h"
#include <string>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
using namespace std;

const char SECRET_KEY = 'K';  // Simple key for XOR operation

inline string genderToString(Gender gender) {
    switch (gender) {
    case male: return "Male";
    case female: return "Female";
    default: return "N/A";
    }
}

inline string religionToString(Religion religion) {
    switch (religion) {
    case Islam: return "Islam";
    case Atheism: return "Atheism";
    case Buddhism: return "Buddhism";
    case Christianity: return "Christianity";
    case Hinduism: return "Hinduism";
    case Judaism: return "Judaism";
    case Sikhism: return "Sikhism";
    case Zoroastrianism: return "Zoroastrianism";
    case Other: return "Other";
    default: return "Unknown";
    }
}

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

#endif
