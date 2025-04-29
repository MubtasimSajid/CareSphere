#pragma once
#include "Patient.h"
#include "Database.h"
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class PatientDashboard {
private:
    Patient* patient;  // Using Patient pointer instead of individual fields

    // Helper methods
    double calculateBMI() const;
    string getBMICategory(double bmi) const;
    bool isHighBloodPressure() const;
    bool isLowBloodPressure() const;
    bool isDiabetic() const;
    string getCurrentDateTime() const;
    bool validateAppointmentDateTime(const string& date, const string& time) const;
    double calculateAppointmentFee(int doctorId) const;
    int calculateAge() const;
    void loadPatientData(int userId);
    string getDepartmentName(MYSQL* conn, int departmentId);

public:
    // Constructor declaration only (no implementation)
    PatientDashboard(int userId);
    
    // Destructor
    ~PatientDashboard();

    // Main dashboard interface
    void displayDashboard();

    // Dashboard options
    void showProfile() const;
    void showMedicalInfo() const;
    void showAdvancedAnalysis() const;
    void makeNote();
    void showNotes() const;
    void showSchedule() const;
    void makeAppointment();
    void showUnpaidBills() const;
    void payBill(int billId);

    // Medication methods
    void showMedications() const;

    // New methods
    void rateDoctor(int doctorId);
    void rateHospital(int hospitalId);
    bool validateDepartment(int departmentId) const;
    bool validateDoctor(int doctorId, int departmentId) const;
    bool validateHospital(int hospitalId, int doctorId) const;
}; 