#include "user.h"
#include<iostream>
#include<string>
using namespace std;

User::User(string id, string name, string email, string Phone_NO, Gender gender, Religion religion, string DOB)
{
    this->name = name;
    this->id = id;
    this->email=email;
    this->Phone_NO=Phone_NO;
    this->gender=gender;
    this->religion=religion;
    this->DOB=DOB;
}

string User::getId()
{
    return id;
}

void User::setId(const string &inputID)
{
    id = inputID;
}

string User::getName()
{
    return name;
}

void User::setName(const string &inputName)
{
    name = inputName;
}

string User::getemail()
{
    return email;
}

void User::set_email(string email)
{
    this->email =email;
}

string User::get_Phone_No()
{
    return Phone_NO;
}

void User::set_Phone_No(string Phone_No)
{
    this->Phone_NO = Phone_No;
}

Gender User::get_Gender()
{
    return gender;
}

void User::set_Gender(Gender InputGender)
{
    gender = InputGender;
}

Religion User::get_Religion()
{
    return religion;
}

void User::set_Religion(Religion InputReligion)
{
    religion = InputReligion;
}

string User::get_DOB()
{
    return DOB;
}

void User::set_DOB(string Input_DOB)
{
    DOB= Input_DOB;
}
