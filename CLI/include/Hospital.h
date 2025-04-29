#pragma once
#include "User.h"
#include <string>
#include <vector>
#include <mysql.h>

class Hospital : public User {
private:
    string hospital_name;
    string address;
    string city;
    string state;
    string country;
    string pincode;
    bool has_ambulance;
    double income;
    double expenditure;
    double average_rating;
    int total_ratings;
    vector<int> department_ids;
    vector<int> doctor_ids;

    // Helper functions
    bool validatePincode(const string& pincode);
    bool validateAddress(const string& address);
    bool validatePhone(const string& phone);

public:
    Hospital();  // Constructor declaration only

    // Getters
    string getHospitalName() const { return hospital_name; }
    string getAddress() const { return address; }
    string getCity() const { return city; }
    string getState() const { return state; }
    string getCountry() const { return country; }
    string getPincode() const { return pincode; }
    bool getHasAmbulance() const { return has_ambulance; }
    double getIncome() const { return income; }
    double getExpenditure() const { return expenditure; }
    double getAverageRating() const { return average_rating; }
    int getTotalRatings() const { return total_ratings; }
    const vector<int>& getDepartmentIds() const { return department_ids; }
    const vector<int>& getDoctorIds() const { return doctor_ids; }

    // Setters
    void setHospitalName(const string& name) { hospital_name = name; }
    void setAddress(const string& addr) { address = addr; }
    void setCity(const string& c) { city = c; }
    void setState(const string& s) { state = s; }
    void setCountry(const string& c) { country = c; }
    void setPincode(const string& p) { pincode = p; }
    void setHasAmbulance(bool has) { has_ambulance = has; }
    void setIncome(double inc) { income = inc; }
    void setExpenditure(double exp) { expenditure = exp; }
    void setAverageRating(double rating) { average_rating = rating; }
    void setTotalRatings(int ratings) { total_ratings = ratings; }

    // Hospital management functions
    bool addDepartment(int departmentId);
    bool removeDepartment(int departmentId);
    bool addDoctor(int doctorId);
    bool removeDoctor(int doctorId);
    bool updateRating(double newRating);
    bool updateFinancials(double income, double expenditure);
    void displayInfo() const override;
    bool displayDepartments() const;
    bool displayDoctors() const;
    bool displayAmbulances() const;
    bool displayMedicalServices() const;

    // Implementation of pure virtual functions from User class
    bool registerUser() override;
    bool login(const string& email, const string& password) override;
    bool loadHospitalData(int userId);  // New method to load hospital data by user ID
    bool logout() override;
    bool updateProfile() override;
}; 