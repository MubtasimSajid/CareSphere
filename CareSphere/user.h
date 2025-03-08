#ifndef USER_H
#define USER_H
#include<iostream>
#include<string>
#include <vector>
using namespace std;

enum Gender {
    male,
    female,
    N_A
};
enum Religion {
    Islam,
    Atheism,
    Buddhism,
    Christianity,
    Hinduism,
    Judaism,
    Sikhism,
    Zoroastrianism,
    Other
};

class User
{
private:
    string id;
    string name;
    string email;
    string phoneNo;
    Gender gender;
    Religion religion;
    string DOB;
public:
    User(string id, string name, string email = "", string phoneNo = "", Gender gender = N_A, Religion religion = Other, string DOB = "");
    string getId() const;
    void setId(const string &inputID);
    string getName() const;
    void setName(const string &inputName);
    string getEmail() const;
    void setEmail(string email);
    string getPhoneNo() const;
    void setPhoneNo(string phoneNo);
    Gender getGender() const;
    void setGender(Gender inputGender);
    Religion getReligion() const;
    void setReligion(Religion inputReligion);
    string getDOB() const;
    void setDOB(string inputDOB);

};

void saveUsers(const vector<User>& users);
vector<User> loadUsers();

#endif
