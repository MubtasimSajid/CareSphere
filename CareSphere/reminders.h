#ifndef REMINDERS_H
#define REMINDERS_H

#include <string>

using namespace std;

class Reminder {
private:
    string user_name;
    string title;
    string date;
    string time;
    string note;

public:
    // Constructors
    Reminder();
    Reminder(string un, string t, string d, string tm, string n);

    // Setters
    void setUserName (string un);
    void setTitle(string t);
    void setDate(string d);
    void setTime(string tm);
    void setNote(string n);

    // Getters
    string getUserName() const;
    string getTitle() const;
    string getDate() const;
    string getTime() const;
    string getNote() const;

    // Display Function
    string formatReminder() const;
};


void Save_User_Reminder(Reminder r);
string Get_User_Reminders (const string &user_name);
#endif // REMINDERS_H
