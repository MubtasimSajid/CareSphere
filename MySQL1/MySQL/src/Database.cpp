#include "../include/Database.h"
#include <iostream>

// Initialize static members
Database* Database::instance = nullptr;
MYSQL* Database::conn = nullptr;
const char* Database::HOST = "localhost";
const char* Database::USER = "root";
const char* Database::PASSW = "Mahkib319";
const char* Database::DB = "CareSphere";

Database::Database() {
    connect();
}

Database::~Database() {
    closeConnection();
}

bool Database::connect() {
    try {
        std::cout << "Initializing MySQL connection...\n";
        conn = mysql_init(nullptr);
        if (!conn) {
            throw std::runtime_error("MySQL initialization failed: " + std::string(mysql_error(conn)));
        }

        std::cout << "Attempting to connect to database...\n";
        if (!mysql_real_connect(conn, HOST, USER, PASSW, DB, 3306, nullptr, 0)) {
            throw std::runtime_error("Connection failed: " + std::string(mysql_error(conn)));
        }

        std::cout << "Connected to database successfully!\n";
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        if (conn) {
            std::cerr << "MySQL Error: " << mysql_error(conn) << std::endl;
            std::cerr << "MySQL Error Code: " << mysql_errno(conn) << std::endl;
        }
        return false;
    }
}

void Database::closeConnection() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}

Database& Database::getInstance() {
    if (instance == nullptr) {
        instance = new Database();
    }
    return *instance;
}

MYSQL* Database::getConnection() {
    return getInstance().conn;
}

bool Database::executeQuery(const std::string& query) {
    MYSQL* connection = getConnection();
    if (!connection) {
        std::cerr << "No database connection" << std::endl;
        return false;
    }
    
    if (mysql_query(connection, query.c_str())) {
        std::cerr << "Query error: " << mysql_error(connection) << std::endl;
        return false;
    }
    return true;
} 






//mm