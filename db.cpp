#include "db.h"
#include "sqlite3.h"
#include <fstream>
#include <string>
#include <iostream>


using namespace WSE;
using namespace std;


bool DB::create()
{
    // Pointer to SQLite connection
    sqlite3 *db;

    // Save any SQL
    const char *sql;

    // Save the result of opening the file
    int rc = sqlite3_open("data.db", &db);

    if( rc ){
        // Show an error message
        cout << "DB Error: " << sqlite3_errmsg(db) << endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return false;
    }

    // Save SQL insert data
    sql = "CREATE TABLE WATCHES("
          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
          "Title        TEXT     NOT NULL,"
          "Description  TEXT     NULL);";

    // Run the SQL
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    // Close the SQL connection
    sqlite3_close(db);

    return true;

}


bool DB::insert(string title, string description)
{
    // Pointer to SQLite connection
    sqlite3 *db;

    // Save any SQL
    string sql;

    // Save the result of opening the file
    int rc = sqlite3_open("data.db", &db);

    if( rc ){
        // Show an error message
        cout << "DB Error: " << sqlite3_errmsg(db) << endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return false;
    }

    // Save SQL insert data
    sql = "INSERT INTO Watches (Title,Description) VALUES('" + title + "','" + description + "');";

    // Run the SQL (convert the string to a C-String with c_str() )
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);

    // Close the SQL connection
    sqlite3_close(db);

    return true;
}

// Create a callback function
int fetch_callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    for(int i = 0; i < argc; i++) {

        // Show column name, value, and newline
        cout << azColName[i] << ": " << argv[i] << endl;
    }

    // Insert a newline
    cout << endl;

    // Return successful
    return 0;
}


bool DB::search(string query)
{
    // Pointer to SQLite connection
    sqlite3 *db;

    // Save any error messages
    char *zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    // Save any SQL
    string sql;

    // Save the result of opening the file
    rc = sqlite3_open("data.db", &db);

    if( rc ){
        // Show an error message
        cout << "DB Error: " << sqlite3_errmsg(db) << endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return false;
    }

    // Save SQL insert data
    sql = "SELECT * FROM Watches WHERE Title LIKE '%" + query + "%';";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), fetch_callback, 0, &zErrMsg);

    // Close the SQL connection
    sqlite3_close(db);

    return true;
}


