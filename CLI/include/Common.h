#pragma once

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <mysql.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

// Macro for MySQL query error checking
#define MYSQL_CHECK_ERROR(conn, query) \
    if (mysql_query(conn, query.c_str())) { \
        cerr << "MySQL Error: " << mysql_error(conn) << endl; \
        return false; \
    } 

// Function to get password with asterisk masking
inline string getSecurePassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {  // Read until Enter is pressed
        if (ch == '\b') {  // Handle backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";  // Erase the last asterisk
            }
        }
        else {
            password += ch;
            cout << '*';  // Print asterisk instead of the actual character
        }
    }
    cout << endl;
    return password;
} 