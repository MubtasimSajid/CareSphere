#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

using namespace std;

class Appointment {
private:
    string user_name;  // Backend usage, not displayed
    string doctor_name;
    string location;
    string date;
    string time;

public:
    // Constructors
    Appointment();
    Appointment(string un, string dn, string loc, string d, string t);

    // Setters
    void setUserName(string un);
    void setDoctorName(string dn);
    void setLocation(string loc);
    void setDate(string d);
    void setTime(string t);

    // Getters
    string getUserName() const;
    string getDoctorName() const;
    string getLocation() const;
    string getDate() const;
    string getTime() const;

    // Format output
    string formatAppointment() const;
};
void Save_User_Appointment(Appointment a);
string Get_User_Appointments (const string &user_name);
#endif // APPOINTMENT_H
