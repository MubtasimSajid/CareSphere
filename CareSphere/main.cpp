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

    // Create a User object with test values
    User user1("U123", "John Doe", "johndoe@example.com", "1234567890", male, Christianity, "01-01-1990");
    // Display user details using getter methods
    cout << "User ID: " << user1.getId() << endl;
    cout << "Name: " << user1.getName() << endl;
    cout << "Email: " << user1.getemail() << endl;
    cout << "Phone Number: " << user1.get_Phone_No() << endl;
    cout << "Gender: " << genderToString(user1.get_Gender()) << endl;
    cout << "Religion: " << religionToString(user1.get_Religion()) << endl;
    cout << "Date of Birth: " << user1.get_DOB() << endl;

    // Modify user details using setter methods
    user1.setName("Jane Doe");
    user1.set_email("janedoe@example.com");
    user1.set_Phone_No("9876543210");
    user1.set_Gender(female);
    user1.set_Religion(Hinduism);
    user1.set_DOB("02-02-1992");

    // Display updated details
    cout << "\nUpdated User Information:\n";
    cout << "Name: " << user1.getName() << endl;
    cout << "Email: " << user1.getemail() << endl;
    cout << "Phone Number: " << user1.get_Phone_No() << endl;
    cout << "Gender: " << genderToString(user1.get_Gender()) << endl;
    cout << "Religion: " << religionToString(user1.get_Religion()) << endl;
    cout << "Date of Birth: " << user1.get_DOB() << endl;



    return a.exec();
}
