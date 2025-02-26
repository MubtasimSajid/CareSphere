#ifndef LOGIN_H
#define LOGIN_H
#include<iostream>
#include <string>
#include <vector>
#include <utility>  // Correct include for std::pair
// #include"login.h"
using namespace std;
class Login
{
private:
    vector<pair<string, string>> credentials; // Corrected syntax

public:
    Login();
    void Register(string name, string password);
    bool verify(string name, string password);
};


#endif // LOGIN_H
