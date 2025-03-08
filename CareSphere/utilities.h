#ifndef UTILITIES_H
#define UTILITIES_H
#include<string>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
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

void connectAndRunQuery() {
    // Create a MySQL database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("CareSphereDB"); // Database created in Step 1
    db.setUserName("root"); // Replace with your MySQL username
    db.setPassword("sql1234567890#SQL"); // Replace with your MySQL password
    db.setPort(3306); // MySQL default port

    // Try to open the database
    if (!db.open()) {
        qDebug() << "❌ Database connection failed:" << db.lastError().text();
        return;
    }

    qDebug() << "✅ Connected to MySQL Database Successfully!";

    // Create a SQL query
    QSqlQuery query;
    QString insertQuery = "INSERT INTO users (name, email, password) VALUES ('John Doe', 'john@example.com', 'securepassword')";

    if (query.exec(insertQuery)) {
        qDebug() << "✅ User inserted successfully!";
    } else {
        qDebug() << "❌ Failed to insert user:" << query.lastError().text();
    }
}
#endif // UTILITIES_H
