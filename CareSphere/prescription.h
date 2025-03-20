#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <string>
#include <vector>
#include <QDebug>
#include <fstream>
#include <vector>
#include <sstream>   // String stream for CSV parsing
#include <sys/stat.h> // For creating directories
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <iomanip> // For table formatting
#include "MySQL_utilities.h"
#include "utilities.h"

using namespace std;

class Prescription {
private:
    string user_name;
    string doctor_name;
    string medicines;

public:
    Prescription();
    Prescription(string un, string dn, string m);

    void setUserName(string un);
    void setDoctorName(string dn);
    void setMedicineNotes(string m);

    string getUserName() const;
    string getDoctorName() const;
    string getMedicineNotes() const;
};

void SavePrescription(const Prescription &p);
vector<string> GetUserPrescriptions(const string &user_name);
void UpdatePrescription(const string& user_name, const string& doctor__Name_new, const string& medicine_Text_new, const string& doctor__Name_old, const string& medicine_Text_old);
void DeletePrescription(const string& user_name, const string& doctor__Name, const string& medicine_Text);

#endif
