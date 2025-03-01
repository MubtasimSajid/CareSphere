#include "user.h"
#include <iostream>
#include <fstream>   // File handling
#include <vector>    // To store multiple users
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
void save_users(const vector<User>& users) {
    createFolderIfNotExists();  // Ensure folder exists

    ofstream file(FILE_NAME);  // Open file for writing
    if (!file) {
        cout << "Error: Could not open file for writing!" << endl;
        return;
    }

    // Write CSV Header
    file << "ID,Name,Email,Phone Number,Gender,Religion,DOB\n";

    // Write user details
    for (const auto& user : users) {
        file << user.getId() << ","
             << user.getName() << ","
             << user.getemail() << ","
             << user.get_Phone_No() << ","
             << user.get_Gender() << ","
             << user.get_Religion() << ","
             << "\"" << user.get_DOB() << "\"" << "\n";  // Wrap DOB in quotes
    }

    file.close();
    cout << "User details saved successfully to " << FILE_NAME << endl;
}


// Load users from the CSV file
vector<User> load_users() {
    vector<User> users;
    ifstream file(FILE_NAME);  // Open file for reading

    if (!file) {
        cout << "Error: Could not open file for reading!" << endl;
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
    cout << "User details loaded successfully from " << FILE_NAME << endl;
    return users;
}


User::User(string id, string name, string email, string Phone_NO, Gender gender, Religion religion, string DOB)
{
    if (id.empty() || name.empty() || Phone_NO.empty()) {
        cout << "Error: User object cannot be initialized without an ID, Name, and Phone Number!" << endl;
        return; // Exit constructor early
    }

    this->id = id;
    this->name = name;
    this->email = email;
    this->Phone_NO = Phone_NO;
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

string User::getemail() const
{
    return email;
}

void User::set_email(string email)
{
    this->email =email;
}

string User::get_Phone_No() const
{
    return Phone_NO;
}

void User::set_Phone_No(string Phone_No)
{
    this->Phone_NO = Phone_No;
}

Gender User::get_Gender() const
{
    return gender;
}

void User::set_Gender(Gender InputGender)
{
    gender = InputGender;
}

Religion User::get_Religion() const
{
    return religion;
}

void User::set_Religion(Religion InputReligion)
{
    religion = InputReligion;
}

string User::get_DOB() const
{
    return DOB;
}

void User::set_DOB(string Input_DOB)
{
    DOB= Input_DOB;
}
