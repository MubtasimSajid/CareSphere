#include <bits/stdc++.h>
using namespace std;

bool loggedIn = false;
string currentID = "";

enum Gender {
    male,
    female
};

enum Religion {
    Islam,
    Atheism,
    Buddhism,
    Christianity,
    Hinduism,
    Judaism,
    Sikhism,
    Zoroastrianism,
    Other
};

class User {
    private:
        string id;
        string name;
        string password;
        vector<string> phoneNo;
        vector<string> email;
        string dateOfBirth;
        Gender gender;
        Religion religion;
        vector<string> socialMedia;

    public:
        User(const string &id, const string &name, const string &password, const vector<string> &phoneNo,
             const vector<string> &email, const string &dateOfBirth, Gender gender, Religion religion,
             const vector<string> &socialMedia)
            : id(id), name(name), password(password), phoneNo(phoneNo), email(email), dateOfBirth(dateOfBirth),
              gender(gender), religion(religion), socialMedia(socialMedia) {}
        virtual ~User() {}

        string getId() const { return id; }
        void setId(const string &inputID) { id = inputID; }

        string getPassword() const { return password; }
        void setPassword(const string &inputPassword) { password = inputPassword; }

        string getName() const { return name; }
        void setName(const string &inputName) { name = inputName; }

        vector<string> getPhoneNo() const { return phoneNo; }
        void setPhoneNo(const vector<string> &inputPhone) { phoneNo = inputPhone; }
        void addPhoneNo(const string &inputPhone) { phoneNo.push_back(inputPhone); }
        void removePhoneNo(const string &numberToRemove) {
            phoneNo.erase(remove(phoneNo.begin(), phoneNo.end(), numberToRemove), phoneNo.end());
        }

        vector<string> getEmail() const { return email; }
        void setEmail(const vector<string> &inputMail) { email = inputMail; }
        void addEmail(const string &inputMail) { email.push_back(inputMail); }
        void removeEmail(const string &emailToRemove) {
            email.erase(remove(email.begin(), email.end(), emailToRemove), email.end());
        }

        string getDateOfBirth() const { return dateOfBirth; }
        void setDateOfBirth(const string &dob) { dateOfBirth = dob; }

        Gender getGender() const { return gender; }
        void setGender(Gender inputGender) { gender = inputGender; }

        Religion getReligion() const { return religion; }
        void setReligion(Religion inputReligion) { religion = inputReligion; }

        vector<string> getSocialMedia() const { return socialMedia; }
        void setSocialMedia(const vector<string> &inputSocials) { socialMedia = inputSocials; }
        void addSocialMedia(const string &inputSocial) { socialMedia.push_back(inputSocial); }
        void removeSocialMedia(const string &socialToRemove) {
            socialMedia.erase(remove(socialMedia.begin(), socialMedia.end(), socialToRemove), socialMedia.end());
        }
};

class HealthCareSystem {
    private:
        string databaseFile = "users.txt";

        void writeToFile(const User &user) {
            ofstream file(databaseFile, ios::app);
            if (file.is_open()) {
                file << user.getId() << ", " << user.getPassword() << endl;
                file.close();
            } else {
                cerr << "Error: Could not open the desired file" << endl;
            }
        }

        bool userExists(const string &id) {
            ifstream file(databaseFile);
            string line;
            if (file.is_open()) {
                while (getline(file, line)) {
                    stringstream ss(line);
                    string userId;
                    getline(ss, userId, ',');
                    if (userId == id) {
                        file.close();
                        return true;
                    }
                }
                file.close();
            }
            return false;
        }

        bool validateLogin(const string &id, const string &password) {
            ifstream file(databaseFile);
            string line;
            if (file.is_open()) {
                while (getline(file, line)) {
                    stringstream ss(line);
                    string userId, userPassword;
                    getline(ss, userId, ',');
                    getline(ss, userPassword, ',');
                    if (userId == id && userPassword == password) {
                        file.close();
                        currentID = id;
                        loggedIn = true;
                        return true;
                    }
                }
                file.close();
            }
            return false;
        }

        public:
            void registerUser() {
                string id, name, password, dob;
                int genderInput, religionInput;
                Gender gender;
                Religion religion;

                cout << "Enter ID: ";
                cin >> id;

                if (userExists(id)) {
                    cout << "User with this ID already exists" << endl;
                    return;
                }

                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter password: ";
                cin >> password;

                cout << "Enter date of birth (YYYY-MM-DD): ";
                cin >> dob;

                cout << "Enter your gender (0 for male, 1 for female): ";
                cin >> genderInput;
                gender = static_cast<Gender>(genderInput);

                cout << "Enter your religion (0-8 for Islam, Atheism, Buddhism, Christianity, Hinduism, Judaism, Sikhism, Zoroastrianism, Other): ";
                cin >> religionInput;
                religion = static_cast<Religion>(religionInput);

                User user(id, name, password, {}, {}, dob, gender, religion, {});
                writeToFile(user);

                cout << "User registration successful!" << endl;
                return;
            }

            void login() {
                if (loggedIn) {
                    cout << "Error: You are already logged in" << endl;
                    return;
                }
                string id, password;

                cout << "Enter ID: ";
                cin >> id;

                cout << "Enter password: ";
                cin >> password;

                if (validateLogin(id, password)) {
                    cout << "Login successful!" << endl;
                } else {
                    cout << "Invalid ID or password." << endl;
                }
            }

            void showUserDetails(const User &user) {
                cout << "\n--- User Details ---\n";
                cout << "ID: " << user.getId() << endl;
                cout << "Name: " << user.getName() << endl;
                cout << "Date of Birth: " << user.getDateOfBirth() << endl;
                cout << "Gender: " << (user.getGender() == male ? "Male" : "Female") << endl;
                cout << "Religion: " << user.getReligion() << endl;
            }
};

void showUserDetails(const string &id = currentID) {
    
}

int main(void) {
    HealthCareSystem hcs;
    int input;

    while(true) {
        if (!loggedIn) {
            currentID = "";
        }

        cout << "\nWelcome to CareSphere\n";
        cout << "\n1. Register User\n2. Login\n3. Log-out\n4. Exit\nEnter your choice: ";
        cin >> input;

        switch (input)
        {
        case 1:
            if (loggedIn) {
                cout << "Error: You are already logged in. Please log out first." << endl;
                break;
            }
            hcs.registerUser();
            break;
        case 2:
            hcs.login();
            break;
        case 3:
            if (loggedIn) {
                loggedIn = false;
                cout << "Logged out successfully!" << endl;
            } else {
                cout << "You are not logged in" << endl;
            }
            break;
        case 4:
            cout << "Exiting the system..." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}