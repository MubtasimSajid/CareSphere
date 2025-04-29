#pragma once
#include "User.h"
#include <vector>
#include <string>

class Doctor : public User {
private:
    string specialization;
    double appointment_fees;
    int experience_years;
    vector<int> hospital_ids;
    vector<string> degrees;
    int doctor_id;
    double consultation_fee;
    vector<string> hospitals;
    string license_number;

    // Helper functions
    bool isValidEmail(const string& email);
    bool isValidPassword(const string& password);
    bool isValidPhone(const string& phone);
    bool isValidDateOfBirth(const string& dob);
    string getDepartmentName(int department_id);

public:
    Doctor();

    // Getters
    string getSpecialization() const { return specialization; }
    double getAppointmentFees() const { return appointment_fees; }
    int getExperienceYears() const { return experience_years; }
    const vector<string>& getDegrees() const { return degrees; }
    const vector<string>& getHospitals() const { return hospitals; }
    string getLicenseNumber() const { return license_number; }
    double getConsultationFee() const { return consultation_fee; }
    const vector<int>& getHospitalIds() const { return hospital_ids; }
    int getDoctorId() const { return doctor_id; }

    // Setters
    void setSpecialization(const string& spec) { specialization = spec; }
    void setAppointmentFees(double fees) { appointment_fees = fees; }
    void setExperienceYears(int years) { experience_years = years; }
    void setLicenseNumber(const string& license) { license_number = license; }
    void setConsultationFee(double fee) { consultation_fee = fee; }

    // Degree management
    bool addDegree(const string& degree);
    bool removeDegree(const string& degree);

    // Hospital management
    bool addHospital(int hospitalId);
    bool removeHospital(int hospitalId);

    // Implementation of pure virtual functions
    bool registerUser() override;
    bool updateProfile() override;
    void displayInfo() const override;

    // Doctor-specific methods
    bool prescribeMedication(int patientId, const string& medicationName, 
                           const string& dosage, const string& duration);
    bool approveAppointment(int appointmentId);
    bool rejectAppointment(int appointmentId);
    bool updateSchedule(const string& date, const string& time, bool isAvailable);
    bool viewAppointments() const;
}; 