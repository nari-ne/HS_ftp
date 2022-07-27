#pragma once

#include "sqlite3.h"

#include <set>
#include <string>

class SQLite_DB
{
public:
  // row structure of database table
  struct Record {
    int id;
    std::string name;
    std::string country;
    std::string email;
  };

public:
  SQLite_DB(const std::string& name);
  ~SQLite_DB();

  bool create_database();                          // Create a new SQLite database connection
  bool generate_data(int row_count);               // Create table and insert row_count rows with random data to it
  bool dump_to_file(const std::string& filename);  // Dump database to a file
  
  void drop_table() const;                         // Added for Unit test purposes

private:
  bool create_table() const;
  bool fillup_table(int row_count);
  Record generate_record() const;
  bool add_record(const Record& rec) const;
  void disconnect() const;                         // Close database

  void report_sqlite_error(const std::string& file, int line) const; // Helps to get the exact location of an error 

private:
  sqlite3* m_db_ptr;                 // SQLite db handle 
  const std::string m_name;          // Database filename (UTF-8)
  const std::string m_table_name;    // Table name
                                     
  static int s_record_id;            // Unique id for each record
};
