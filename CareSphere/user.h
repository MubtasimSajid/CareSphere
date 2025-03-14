#ifndef USER_H
#define USER_H
#include<iostream>
#include<string>
#include <vector>
using namespace std;

class User
{
private:
    string id;
    string name;
    string email;
    string phoneNo;
    string gender;
    string DOB;
    string bloodGroup;
public:
    User(string id, string name, string email, string phoneNo, string bloodGroup, string gender = "Male", string DOB = "");
    string getId() const;
    void setId(const string &inputID);
    string getName() const;
    void setName(const string &inputName);
    string getEmail() const;
    void setEmail(string email);
    string getPhoneNo() const;
    void setPhoneNo(string phoneNo);
    string getGender() const;
    void setGender(string inputGender);
    string getDOB() const;
    void setDOB(string inputDOB);
    string getBloodGroup() const;
    void setBloodGroup(const string &bg);
    void details();
};

void saveUser(const User &user);
vector<User> loadUsers();
User getUser(const string &username);

#endif
