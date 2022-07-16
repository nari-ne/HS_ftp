#pragma once

#include <string>
#include <set>
#include "sqlite3.h"

//row structure of db table
struct db_record {
  int id;
  std::string name;
  std::string country;
  std::string email;
};


//class SQLite_DB: definition (SQLite database)
class SQLite_DB
{
public:
  SQLite_DB(const std::string& n);

  bool create_sql_database(); // Create a new SQLite database connection and generate data for database
  bool generate_data(int r);  //create table and insert r rows with random data to it
  bool dump_to_file(const std::string& path); // Dump database to a file
  void disconnect();

  bool create_table(const std::string& table_name);
  bool fillup_table(const std::string& table_name, int r);
  db_record generate_record();
  bool add_record(const db_record& rec, const std::string& table_name);
  void report_sqlite_error(const std::string& file, int line) const; // Helps to get exact location of an error 

private:
  sqlite3* m_db_ptr;              // SQLite db handle 
  const std::string m_name;       // Database name
  const std::string m_file_name;  // Database filename (UTF-8)
  static int m_id;
};


