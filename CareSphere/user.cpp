#include "user.h"
#include <QDebug>
#include <fstream>
#include <vector>
#include <sstream>   // String stream for CSV parsing
#include <sys/stat.h> // For creating directories

using namespace std;
// Define the folder and file path
const string FOLDER_NAME = "User Details";
const string FILE_NAME = FOLDER_NAME + "/users.csv";


// Function to create the folder if it doesn't exist
void createFolderIfNotExists() {
    struct stat info;
    if (stat(FOLDER_NAME.c_str(), &info) != 0) {
        system(("mkdir \"" + FOLDER_NAME + "\"").c_str());  // Create folder (Windows/Linux)
    }
}
// Save multiple users to a CSV file
void saveUsers(const vector<User>& users) {
    createFolderIfNotExists();  // Ensure folder exists

    ofstream file(FILE_NAME);  // Open file for writing
    if (!file) {
        qInfo() << "Error: Could not open file for writing!\n";
        return;
    }

    // Write CSV Header
    file << "ID,Name,Email,Phone Number,Gender,Religion,DOB\n";

    // Write user details
    for (const auto& user : users) {
        file << user.getId() << ","
             << user.getName() << ","
             << user.getEmail() << ","
             << "\"=\"" << user.getPhoneNo() << "\"\","  // Fixes phone number gibberish
             << user.getGender() << ","
             << user.getReligion() << ","
             << "\"=\"" << user.getDOB() << "\"\""  // Fixes DOB hash issue
             << "\n";
    }

    file.close();
    qInfo() << "User details saved successfully to " << FILE_NAME << "\n";
}



// Load users from the CSV file
vector<User> loadUsers() {
    vector<User> users;
    ifstream file(FILE_NAME);  // Open file for reading

    if (!file) {
        qInfo() << "Error: Could not open file for reading!\n";
        return users;
    }

    string line, id, name, email, phone, dob;
    int gender, religion;

    getline(file, line);  // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, phone, ',');
        getline(ss, temp, ','); gender = stoi(temp);
        getline(ss, temp, ','); religion = stoi(temp);
        getline(ss, dob, ',');

        // Create User object and add to vector
        users.push_back(User(id, name, email, phone, static_cast<Gender>(gender), static_cast<Religion>(religion), dob));
    }

    file.close();
    qInfo() << "User details loaded successfully from " << FILE_NAME << "\n";
    return users;
}


User::User(string id, string name, string email, string phoneNo, Gender gender, Religion religion, string DOB)
{
    if (id.empty() || name.empty() || phoneNo.empty()) {
        qInfo() << "Error: User object cannot be initialized without an ID, Name, and Phone Number!\n";
        return;
    }

    this->id = id;
    this->name = name;
    this->email = email;
    this->phoneNo = phoneNo;
    this->gender = gender;
    this->religion = religion;
    this->DOB = DOB;
}


string User::getId() const
{
    return id;
}

void User::setId(const string &inputID)
{
    id = inputID;
}

string User::getName() const
{
    return name;
}

void User::setName(const string &inputName)
{
    name = inputName;
}

string User::getEmail() const
{
    return email;
}

void User::setEmail(string email)
{
    this->email = email;
}

string User::getPhoneNo() const
{
    return phoneNo;
}

void User::setPhoneNo(string phoneNo)
{
    this->phoneNo = phoneNo;
}

Gender User::getGender() const
{
    return gender;
}

void User::setGender(Gender inputGender)
{
    gender = inputGender;
}

Religion User::getReligion() const
{
    return religion;
}

void User::setReligion(Religion inputReligion)
{
    religion = inputReligion;
}

string User::getDOB() const
{
    return DOB;
}

void User::setDOB(string inputDOB)
{
    DOB = inputDOB;
}
