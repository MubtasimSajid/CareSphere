#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <string>
#include <vector>

using namespace std;

class Prescription {
private:
    string user_name;
    string doctor_name;
    string medicines;

public:
    // Constructors
    Prescription();
    Prescription(string un, string dn, string m);

    // Setters
    void setUserName(string un);
    void setDoctorName(string dn);
    void setMedicineNotes(string m);

    // Getters
    string getUserName() const;
    string getDoctorName() const;
    string getMedicineNotes() const;

    // Display function
    void displayDetails() const;
};

// Function declarations for prescription handling
void SavePrescription(const Prescription &p);
vector<string> GetUserPrescriptions(const string &user_name);

#endif // PRESCRIPTION_H
