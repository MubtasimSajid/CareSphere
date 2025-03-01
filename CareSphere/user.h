#ifndef USER_H
#define USER_H
#include<iostream>
#include<string>
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
    string Phone_NO;
    Gender gender;
    Religion religion;
    string DOB;
public:
    User(string id, string name, string email="", string Phone_NO="", Gender gender=N_A, Religion religion=Other, string DOB="");
    // Getter and Setters
    string getId();
    void setId(const string &inputID);
    string getName();
    void setName(const string &inputName);
    string getemail();
    void set_email(string email);
    string get_Phone_No();
    void set_Phone_No(string Phone_No);
    Gender get_Gender();
    void set_Gender(Gender InputGender);
    Religion get_Religion();
    void set_Religion(Religion InputReligion);
    string get_DOB();
    void set_DOB(string Input_DOB);

};

#endif // USER_H
