#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <vector>
#include <sstream>
#include <string>
#include <vector>
#include "MySQL_utilities.h"


using namespace std;

class Appointment {
private:
    string user_name;
    string doctor_name;
    string location;
    string date;
    string time;

public:
    Appointment();
    Appointment(string un, string dn, string loc, string d, string t);

    void setUserName(string un);
    void setDoctorName(string dn);
    void setLocation(string loc);
    void setDate(string d);
    void setTime(string t);

    string getUserName() const;
    string getDoctorName() const;
    string getLocation() const;
    string getDate() const;
    string getTime() const;

    string formatAppointment() const;
};
void Save_User_Appointment(Appointment a);
vector<string> Get_User_Appointments(const string &user_name);
void Update_User_Appointment(string user_name, string newDoctorName, string newLocation, string newAppointmentDate, string newAppointmentTime,
                             string oldDoctorName, string oldLocation, string oldAppointmentDate, string oldAppointmentTime);
void Delete_User_Appointment (string user_name, string DoctorName, string Location, string AppointmentDate , string AppointmentTime );
#endif
