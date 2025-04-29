#pragma once
#include "Common.h"
#include "Database.h"
#include <string>
#include <vector>
#include <ctime>
#include <mysql.h>

using namespace std;

class Doctor;
class Patient;
class DoctorDashboard;
class PatientDashboard;

class User 
{
    friend class Doctor;
    friend class Patient;
    friend class DoctorDashboard;
    friend class PatientDashboard;

protected:
    int user_id;
    string name;
    string email;
    string phone;
    string password;
    string dob;
    string role;  // "patient", "doctor", "hospital"

    // Protected constructor for base class
    User(const string& role = "") : user_id(0) {}

    // Common validation methods
    bool validateInput(const string& input, const string& type);
    bool validateEmail(const string& email);
    bool validatePhone(const string& phone);
    bool validatePassword(const string& password);
    bool validateName(const string& name);
    bool validateDOB(const string& dob);
    int calculateAge(const string& dob);
    string hashPassword(const string& password);
    bool validateBloodGroup(const string& bloodGroup);
    bool validateGender(const string& gender);
    bool registerPatient(MYSQL* conn, int userId);
    bool registerDoctor(MYSQL* conn, int userId);
    bool registerHospital(MYSQL* conn, int userId);
    bool validateDepartment(MYSQL* conn, int departmentId);
    vector<string> getInputList(const string& prompt);

public:
    virtual ~User() {}

    // Getters
    int getUserId() const { return user_id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getDOB() const { return dob; }
    string getRole() const { return role; }
    string getPassword() const { return password; }

    // Setters
    void setUserId(int id) { user_id = id; }
    void setName(const string& n) { name = n; }
    void setEmail(const string& e) { email = e; }
    void setPhone(const string& p) { phone = p; }
    void setDOB(const string& d) { dob = d; }
    void setRole(const string& r) { role = r; }
    void setPassword(const string& p) { password = hashPassword(p); }

    // Pure virtual functions
    virtual bool registerUser() = 0;
    virtual bool updateProfile() = 0;
    virtual void displayInfo() const = 0;

    // Virtual functions with default implementation
    virtual bool login(const string& email, const string& password);
    virtual bool logout();

    // Common methods
    bool changePassword(const string& oldPassword, const string& newPassword);
    bool updatePhone(const string& newPhone);
    bool updateEmail(const string& newEmail);
}; 