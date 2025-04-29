#pragma once
#include "Doctor.h"
#include "Database.h"
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

class DoctorDashboard {
private:
    Doctor* doctor;  // Using Doctor pointer instead of individual fields
    bool executeQuery(const string& query);
    bool isValidPhone(const string& phone);
    void loadDoctorData(int userId);
    int calculateAge(const string& dob) const;
    double getTotalWithdrawn() const;
    void updateBalance(double amount, const string& type, const string& description);
    string getDepartmentName(int department_id) const;

    // Helper methods
    string getCurrentDateTime() const;

public:
    DoctorDashboard(int userId);  // Declaration only, definition in cpp file
    ~DoctorDashboard() {
        if (doctor) delete doctor;
    }

    // Main dashboard interface
    void displayDashboard();

    // Dashboard options
    void showProfile() const;
    void makeNote();
    void showNotes() const;
    void showSchedule() const;
    void updateAvailability();
    void viewPatients() const;
    void prescribeMedication();
    void showBalance() const;
    void showBalanceHistory() const;
    void withdrawMoney();
    void handleAppointment(int appointmentId, bool approve);
    bool updateProfile();
    bool updateSurgeryFees();
}; 