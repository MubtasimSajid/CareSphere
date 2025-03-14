#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H
#include <string>
using namespace std;
class Prescription {
private:
    string user_name;
    string medicine;
    string dosage;
    string frequency;

public:
    // Constructors
    Prescription();
    Prescription(string un, string m, string d, string f);

    // Setters
    void setUserName(string un);
    void setMedicine(string m);
    void setDosage(string d);
    void setFrequency(string f);

    // Getters
    string getUserName() const;
    string getMedicine() const;
    string getDosage() const;
    string getFrequency() const;

    // Details Function
    void displayDetails() const;
};


void SavePrescription(const Prescription &p);
string GetUserPrescriptions (const string &user_name);

#endif // PRESCRIPTION_H
