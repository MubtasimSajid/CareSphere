#include "user.h"
#include "MySQL_utilities.h"
#include <QDebug>
#include <fstream>
#include <vector>
#include <sstream>   // String stream for CSV parsing
#include <sys/stat.h> // For creating directories

using namespace std;
// Define the folder and file path
const string FOLDER_NAME = "User Details";
const string FILE_NAME = FOLDER_NAME + "/users.csv";


void saveUser(const User &user)
{
    QSqlQuery query;
    QString insert_user = "INSERT INTO users (name, email, phoneNo) VALUES ('"+QString::fromStdString(user.getName())+"', '"+QString::fromStdString(user.getEmail())+"', '"+QString::fromStdString(user.getPhoneNo())+"'  )";
    MySQL_Insert(insert_user);
}


vector<User> loadUsers() {
    vector<User> users;
    QSqlQuery query;

    if (query.exec("SELECT id, name, email, phoneNo, gender, religion, DOB FROM users")) {
        while (query.next()) {
            string id = query.value(0).toString().toStdString();
            string name = query.value(1).toString().toStdString();
            string email = query.value(2).toString().toStdString();
            string phoneNo = query.value(3).toString().toStdString();
            Gender gender = static_cast<Gender>(query.value(4).toInt()); // Convert int to Gender enum
            Religion religion = static_cast<Religion>(query.value(5).toInt()); // Convert int to Religion enum
            string DOB = query.value(6).toString().toStdString();

            users.push_back(User(id, name, email, phoneNo, gender, religion, DOB));
        }
    }

    return users;
}



User::User(string id, string name, string email, string phoneNo, Gender gender, Religion religion, string DOB)
{
    if (id.empty() || name.empty() || phoneNo.empty()) {
        return;
    }

    this->id = id;
    this->name = name;
    this->email = email;
    this->phoneNo = phoneNo;
    this->gender = gender;
    this->religion = religion;
    this->DOB = DOB;
}


string User::getId() const
{
    return id;
}

void User::setId(const string &inputID)
{
    id = inputID;
}

string User::getName() const
{
    return name;
}

void User::setName(const string &inputName)
{
    name = inputName;
}

string User::getEmail() const
{
    return email;
}

void User::setEmail(string email)
{
    this->email = email;
}

string User::getPhoneNo() const
{
    return phoneNo;
}

void User::setPhoneNo(string phoneNo)
{
    this->phoneNo = phoneNo;
}

Gender User::getGender() const
{
    return gender;
}

void User::setGender(Gender inputGender)
{
    gender = inputGender;
}

Religion User::getReligion() const
{
    return religion;
}

void User::setReligion(Religion inputReligion)
{
    religion = inputReligion;
}

string User::getDOB() const
{
    return DOB;
}

void User::setDOB(string inputDOB)
{
    DOB = inputDOB;
}

void User::details()
{
    qDebug() << "---------------------------";
    qDebug() << "User Details:";
    qDebug() << "ID:" << QString::fromStdString(id);
    qDebug() << "Name:" << QString::fromStdString(name);
    qDebug() << "Email:" << QString::fromStdString(email);
    qDebug() << "Phone Number:" << QString::fromStdString(phoneNo);
}


