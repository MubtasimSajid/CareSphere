#include <QApplication>
#include "caresphere.h"
#include "login.h"
#include "registration.h"
#include "user.h"
string genderToString(Gender gender) {
    switch (gender) {
    case male: return "Male";
    case female: return "Female";
    default: return "N/A";
    }
}

// Function to convert Religion enum to string
string religionToString(Religion religion) {
    switch (religion) {
    case Islam: return "Islam";
    case Atheism: return "Atheism";
    case Buddhism: return "Buddhism";
    case Christianity: return "Christianity";
    case Hinduism: return "Hinduism";
    case Judaism: return "Judaism";
    case Sikhism: return "Sikhism";
    case Zoroastrianism: return "Zoroastrianism";
    case Other: return "Other";
    default: return "Unknown";
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Login w;
    w.show();

    vector<User> users;

    // **Step 1: Create and Display Users**
    cout << "Creating users..." << endl;
    users.push_back(User("U001", "Alice Johnson", "alice@example.com", "1234567890", female, Christianity, "1995-06-10"));
    users.push_back(User("U002", "Bob Smith", "bob@example.com", "9876543210", male, Islam, "1992-04-15"));
    users.push_back(User("U003", "Charlie Adams", "charlie@example.com", "5551234567", male, Atheism, "1990-12-05"));

    // **Step 2: Save Users to CSV**
    cout << "\nSaving users to CSV..." << endl;
    save_users(users);

    // **Step 3: Load Users from CSV**
    cout << "\nLoading users from CSV..." << endl;
    vector<User> loaded_users = load_users();

    // **Step 4: Display Loaded Users**
    cout << "\nDisplaying loaded users:\n";
    for (const auto& user : loaded_users) {
        cout << "User ID: " << user.getId() << endl;
        cout << "Name: " << user.getName() << endl;
        cout << "Email: " << user.getemail() << endl;
        cout << "Phone Number: " << user.get_Phone_No() << endl;
        cout << "Gender: " << (user.get_Gender() == male ? "Male" : "Female") << endl;
        cout << "Religion: " << user.get_Religion() << endl;  // Enum will return integer
        cout << "Date of Birth: " << user.get_DOB() << endl;
        cout << "---------------------\n";
    }



    return a.exec();
}
