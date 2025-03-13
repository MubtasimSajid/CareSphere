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

#endif
