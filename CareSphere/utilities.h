#ifndef UTILITIES_H
#define UTILITIES_H
#include<string>
using namespace std;
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
#endif // UTILITIES_H
