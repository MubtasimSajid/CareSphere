#pragma once
#include "Hospital.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class HospitalDashboard {
private:
    Hospital* hospital;
    MYSQL* conn;

    // Helper functions
    bool validateDepartment(int departmentId);
    bool validateDoctor(int doctorId);
    bool validateSurgery(int surgeryId);
    bool validateAmbulanceDetails(const string& vehicleNumber, const string& phoneNumber);
    string getDepartmentName(int departmentId);
    string getDoctorName(int doctorId);
    string getSurgeryName(int surgeryId);

public:
    HospitalDashboard(int userId);
    ~HospitalDashboard();

    void displayDashboard();
    void showProfile() const;
    void showDepartments() const;
    void showDoctors() const;
    void showAmbulances() const;
    void showMedicalServices() const;
    void showFinancials() const;
    void showRatings() const;
    void addDepartment();
    void removeDepartment();
    void addDoctor();
    void removeDoctor();
    void addAmbulance();
    void removeAmbulance();
    void addMedicalService();
    void removeMedicalService();
    void updateDepartmentFees();
    void updateSurgeryFees();
    void updateAppointmentFees();
    void viewAppointments() const;
    void viewBills() const;
    void viewPayments() const;
    void generateReport();
}; 