#ifndef MYSQL_UTILITIES_H
#define MYSQL_UTILITIES_H
#include<string>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
using namespace std;
void MySQLinit(){
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
}

void MySQL_Insert(QString query_insert) {
    QSqlQuery query;

    if (query.exec(query_insert)) {
        qDebug() << "✅ Operation inserted successfully!";
    } else {
        qDebug() << "❌ Failed to insert user:" << query.lastError().text();
    }
}

void MySQL_Update(QString query_update) {
    QSqlQuery query;

    if (query.exec(query_update)) {
        qDebug() << "✅ Operation updated successfully!";
    } else {
        qDebug() << "❌ Failed to insert user:" << query.lastError().text();
    }
}


#endif // MYSQL_UTILITIES_H
