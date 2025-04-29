#pragma once
#include <mysql.h>
#include <string>

class Database {
private:
    static Database* instance;
    static MYSQL* conn;
    static const char* HOST;
    static const char* USER;
    static const char* PASSW;
    static const char* DB;
    
    // Private constructor for singleton
    Database();
    
    // Private connect method
    bool connect();
    
public:


    // Delete copy constructor and assignment operator
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    // Static method to get instance
    static Database& getInstance();
    
    // Get database connection
    static MYSQL* getConnection();
    
    // Execute SQL query
    static bool executeQuery(const std::string& query);
    
    // Close database connection
    void closeConnection();
    
    ~Database();
}; 