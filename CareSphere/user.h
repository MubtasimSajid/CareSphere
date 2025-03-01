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
    string Phone_NO;
    Gender gender;
    Religion religion;
    string DOB;
public:
    User(string id, string name, string email="", string Phone_NO="", Gender gender=N_A, Religion religion=Other, string DOB="");
    // Getter and Setters
    string getId() const;
    void setId(const string &inputID);
    string getName() const;
    void setName(const string &inputName);
    string getemail() const;
    void set_email(string email);
    string get_Phone_No() const;
    void set_Phone_No(string Phone_No);
    Gender get_Gender() const;
    void set_Gender(Gender InputGender);
    Religion get_Religion() const;
    void set_Religion(Religion InputReligion);
    string get_DOB() const;
    void set_DOB(string Input_DOB);
    // Files operations functions

};

void save_users(const vector<User>& users);  // Save multiple users to CSV
vector<User> load_users();  // Load users from CSV


#endif // USER_H
