#pragma once
#include "Database.h"
#include <string>
#include <vector>

class User {
private:
    bool validateInput(const std::string& input, const std::string& type);
    std::string hashPassword(const std::string& password);
    int calculateAge(const std::string& dob);
    bool validateBloodGroup(const std::string& bloodGroup);
    bool validateGender(const std::string& gender);
    bool registerPatient(MYSQL* conn, int userId);
    bool registerDoctor(MYSQL* conn, int userId);
    bool registerHospital(MYSQL* conn, int userId);
    bool validateDepartment(MYSQL* conn, int departmentId);
    std::vector<std::string> getInputList(const std::string& prompt);

public:
    bool registerUser();
    bool loginUser();
}; 