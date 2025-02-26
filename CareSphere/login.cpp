#include "login.h"

Login::Login() {
    cout << "Hello From Login Class" << endl;
}

// Function to register a user by saving credentials to a CSV file
void Login::Register(string name, string password) {
    ifstream fileRead(filename);  // Open file to check for existing users
    string line, uname, pass;

    if (fileRead.is_open()) {
        while (getline(fileRead, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');  // Read username
            getline(ss, pass, ',');   // Read password (not needed for checking)

            if (uname == name) {  // If username already exists
                cout << "Error: Username '" << name << "' already exists. Choose a different one.\n";
                fileRead.close();
                return;  // Exit function early
            }
        }
        fileRead.close();
    }

    // If username is not found, proceed with registration
    ofstream fileWrite(filename, ios::out | ios::app);  // Open file in append mode
    if (!fileWrite) {
        cout << "Error: Unable to open file for writing!\n";
        return;
    }
    fileWrite << name << "," << password << "\n";
    fileWrite.flush();
    fileWrite.close();
    cout << "User '" << name << "' registered successfully!\n";
}



// Function to verify user credentials by reading from the CSV file
bool Login::verify(string name, string password) {
    ifstream file(filename);
    string line, uname, pass;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');  // Read username
            getline(ss, pass, ',');   // Read password

            if (uname == name && pass == password) {
                file.close();
                return true; // Match found
            }
        }
        file.close();
    } else {
        cout << "Error: Unable to open file for reading!\n";
    }
    return false; // No match found
}
