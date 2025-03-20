#ifndef REMINDERS_H
#define REMINDERS_H

#include <string>
#include<vector>
using namespace std;

class Reminder {
private:
    string user_name;
    string title;
    string date;
    string time;
    string note;

public:
    Reminder();
    Reminder(string un, string t, string d, string tm = "", string n = "");

    void setUserName (string un);
    void setTitle(string t);
    void setDate(string d);
    void setTime(string tm);
    void setNote(string n);

    string getUserName() const;
    string getTitle() const;
    string getDate() const;
    string getTime() const;
    string getNote() const;

    string formatReminder() const;
};


void Save_User_Reminder(Reminder r);
vector<string> Get_User_Reminders(const string &user_name);
void Update_Reminder(string user_name, string newTitle, string newReminderDate, string newReminderTime, string newNote,
                     string oldTitle, string oldReminderDate, string oldReminderTime, string oldNote);
void Delete_Reminder (string user_name, string Title, string ReminderDate, string ReminderTime, string note);
#endif
