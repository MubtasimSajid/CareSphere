#pragma once

// Undefine Windows macros that conflict with std::
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <mysql.h>
#include <string>

// Macro for MySQL query error checking
#define MYSQL_CHECK_ERROR(conn, query) \
    if (mysql_query(conn, query.c_str())) { \
        cerr << "MySQL Error: " << mysql_error(conn) << endl; \
        return false; \
    } 