#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Login {
private:
    const string filename = "credentials.csv"; // File to store credentials

public:
    Login();
    void Register(string name, string password);
    bool verify(string name, string password);
};

#endif // LOGIN_H
