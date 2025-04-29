#pragma once
#include "User.h"
#include <string>
#include <vector>

class Patient : public User {
private:
    string blood_group;
    string gender;
    string emergency_contact;
    string medical_history;
    string insurance_info;
    double weight;
    double height;
    string blood_pressure;
    double blood_sugar;
    vector<string> allergies;
    vector<string> chronic_conditions;

public:
    Patient();

    // Getters
    string getBloodGroup() const { return blood_group; }
    string getGender() const { return gender; }
    string getEmergencyContact() const { return emergency_contact; }
    string getMedicalHistory() const { return medical_history; }
    string getInsuranceInfo() const { return insurance_info; }
    double getWeight() const { return weight; }
    double getHeight() const { return height; }
    string getBloodPressure() const { return blood_pressure; }
    double getBloodSugar() const { return blood_sugar; }
    const vector<string>& getAllergies() const { return allergies; }
    const vector<string>& getChronicConditions() const { return chronic_conditions; }

    // Setters
    void setBloodGroup(const string& bg) { blood_group = bg; }
    void setGender(const string& g) { gender = g; }
    void setEmergencyContact(const string& ec) { emergency_contact = ec; }
    void setMedicalHistory(const string& mh) { medical_history = mh; }
    void setInsuranceInfo(const string& ii) { insurance_info = ii; }
    void setWeight(double w) { weight = w; }
    void setHeight(double h) { height = h; }
    void setBloodPressure(const string& bp) { blood_pressure = bp; }
    void setBloodSugar(double bs) { blood_sugar = bs; }
    void addAllergy(const string& allergy) { allergies.push_back(allergy); }
    void addChronicCondition(const string& condition) { chronic_conditions.push_back(condition); }

    // Implementation of pure virtual functions
    bool registerUser() override;
    bool updateProfile() override;
    void displayInfo() const override;

    // Patient-specific methods
    bool bookAppointment(int doctor_id, const string& date, const string& time);
    bool cancelAppointment(int appointment_id);
    bool viewAppointments() const;
    bool updateMedicalHistory(const string& new_history);
    bool addInsurance(const string& insurance_details);
    int calculateAge(const string& dob) const;
}; 