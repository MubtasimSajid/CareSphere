#include "login.h"

Login::Login()
 {
    cout<<"Hello From Login Class"<<endl;
}

void Login::Register(string name, string password)
{
    credentials.push_back(make_pair(name, password));

}

bool Login::verify(string name, string password)
{
    for (const auto &cred : credentials) {
        if (cred.first == name && cred.second == password) {
            return true; // Found a match
        }
    }
    return false; // No match found

}
